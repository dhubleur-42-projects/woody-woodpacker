/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compression.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 22:08:53 by jmaia             #+#    #+#             */
/*   Updated: 2023/12/08 00:19:50 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include "compression.h"

#include "libft.h"
#include <stdio.h> //TEMP

static void	find_largest_subbytes(uint8_t *bytes, uint64_t size, uint64_t i_begin, uint64_t *out_i_subbyte, uint64_t *out_length_subbyte);
static int write_represented_data(t_buffered_data *data, uint16_t n_look_behind, uint16_t length_subbyte, uint64_t i_raw_data);
static int write_represented_token_marker(t_buffered_data *data, uint64_t i_raw_data);
static int write_data(t_buffered_data *data, uint8_t *added_data, uint8_t size, uint64_t i_raw_data);
static void flush_buffer(t_buffered_data *data);

/*
 * On va avoir un problème si quand on applique l'algorithme de compression, ça rend le fichier plus lourd qu'avant. On veut l'éviter un max
 * Si jamais on se rend compte que la compression rend le fichier plus lourd qu'avant, on va simplement venir retourner un code d'erreur pour ne pas avoir à gérer ce cas :)
 *
 * À partir de là, comment on peut gérer le fait qu'on va venir écrire au même endroit qu'on va lire ?
 * Go faire ça par bloc je pense en vrai.
 * On fait des blocs de 2k qu'on compresse bout par bout.
 * On va faire exactement pareil que si on avait un fichier !
 * 
 * 1) On lit octet par octet le fichier
 * 2) On construit la compression dans un buffer de 4k
 * 3) À chaque fois qu'on va vouloir écrire la compression, on va vérfier si la longueur du fichier compressé < la longueur du fichier non compressé
 * 3a) Si la longueur est inférieur, on vient écrire les 2k octets les plus vieux
 * 3b) Si la longueur est supérieur, on retourne une erreur, parce que ça veut dire qu'on va écraser de la donnée non compressé pas encore traitée
 */

uint64_t	compress(void *data, uint64_t size)
{
	uint8_t *bytes;
	uint8_t compressed_data[DATA_BUFFER_LENGTH];
	t_buffered_data buffered_data;
	uint64_t i;
	int err;

	bytes = (uint8_t *) data;
	buffered_data.buffer.data = compressed_data;
	buffered_data.buffer.size = DATA_BUFFER_LENGTH;
	buffered_data.buffer.i = 0;

	buffered_data.data.data = bytes;
	buffered_data.data.size = size;
	buffered_data.data.i = 0;
	err = 0;

	i = 0;
	while (i < size && !err)
	{
		uint64_t i_subbyte;
		uint64_t length_subbyte;

		find_largest_subbytes(bytes, size, i, &i_subbyte, &length_subbyte);
		if (LEN_TOKEN <= length_subbyte)
		{
			printf("<%d, %d>", i_subbyte, length_subbyte);
			err = write_represented_data(&buffered_data, i - i_subbyte, length_subbyte, i);
			i += length_subbyte;
		}
		else
		{
			printf("%c", bytes[i]);
			if (bytes[i] == TOKEN_MARKER)
				err = write_represented_token_marker(&buffered_data, i);
			else
				err = write_data(&buffered_data, bytes + i, 1, i);
			i++;
		}
	}
	if (err)
		return UINT64_MAX;
	flush_buffer(&buffered_data);
	return buffered_data.data.i;
}

static void	find_largest_subbytes(uint8_t *bytes, uint64_t size, uint64_t i_begin, uint64_t *out_i_subbyte, uint64_t *out_length_subbyte)
{
	uint64_t best_i_subbyte;
	uint64_t best_length_subbyte;
	uint64_t i_search_haystack;

	best_i_subbyte = -1;
	best_length_subbyte = 0;

	i_search_haystack = (i_begin < SEARCH_BUFFER_LENGTH) ? 0 : (i_begin - SEARCH_BUFFER_LENGTH);
	while (i_search_haystack < i_begin)
	{
		uint64_t i_haystack;
		uint64_t i_needle;
		uint64_t cur_length_subbyte;

		i_haystack = i_search_haystack;
		i_needle = i_begin;
		while (i_haystack < i_begin && i_needle < size && bytes[i_haystack] == bytes[i_needle])
		{
			i_haystack++;
			i_needle++;
		}
		cur_length_subbyte = i_haystack - i_search_haystack;
		if (cur_length_subbyte > best_length_subbyte)
		{
			best_i_subbyte = i_search_haystack;
			best_length_subbyte = cur_length_subbyte;
		}
		i_search_haystack++;
	}
	*out_i_subbyte = best_i_subbyte;
	*out_length_subbyte = best_length_subbyte;
}

static int write_represented_data(t_buffered_data *data, uint16_t n_look_behind, uint16_t length_subbyte, uint64_t i_raw_data)
{
	uint8_t token[LEN_TOKEN];

	token[0] = TOKEN_MARKER;

	token[1] = (n_look_behind & 0xFF00) >> 8;
	token[2] = n_look_behind & 0x00FF;

	token[3] = (length_subbyte & 0xFF00) >> 8;
	token[4] = length_subbyte & 0x00FF;
	return write_data(data, token, LEN_TOKEN, i_raw_data);
}

static int write_represented_token_marker(t_buffered_data *data, uint64_t i_raw_data)
{
	uint8_t token[2];

	token[0] = TOKEN_MARKER;
	token[1] = 0xFF;
	return write_data(data, token, 2, i_raw_data);
}

static int write_data(t_buffered_data *data, uint8_t *added_data, uint8_t size, uint64_t i_raw_data)
{
	if ((data->buffer.i % (data->buffer.size / 2)) + size >= data->buffer.size / 2)
	{
		if (i_raw_data < data->data.i + data->buffer.size / 2)
			return 1; // We are going to overwrite unread data
		flush_buffer(data);
	}
	ft_memcpy(data->buffer.data + data->buffer.i, added_data, size);
	data->buffer.i += size;
	return 0;
}

static void flush_buffer(t_buffered_data *data)
{
	uint8_t *buffer_part_p;
	uint64_t buffer_part_size;
	uint64_t flushed_size;

	buffer_part_size = data->buffer.size / 2;
	flushed_size = data->buffer.i % buffer_part_size;

	if (data->buffer.i < buffer_part_size)
		buffer_part_p = data->buffer.data;
	else
		buffer_part_p = data->buffer.data + buffer_part_size;
	ft_memcpy(data->data.data + data->data.i, buffer_part_p, flushed_size);
	data->data.i += flushed_size;

	if (buffer_part_p == data->buffer.data)
		data->buffer.i = buffer_part_size;
	else
		data->buffer.i = 0;
}

int main(void)
{
	uint8_t data[] =
"I am Sam\n"
"Sam I am\n"
"That Sam-I-am\n"
"That Sam-I-am!\n"
"I do not like\n"
"that Sam-I-am\n"
"Do you like\n"
"green eggs and ham\n"
"I do not like them,\n"
"Sam-I-am.\n"
"I do not like\n"
"green eggs and ham.\n"
"Would you like them\n"
"Here or there?\n"
"I would not like them\n"
"here or there.\n"
"I would not like them\n"
"anywhere.\n"
"I do not like\n"
"green eggs and ham.\n"
"I do not like them,\n"
"Sam-I-am\n"
"Would you like them\n"
"in a house?\n"
"Would you like them\n"
"with a mouse?\n"
"I do not like them\n"
"in a house.\n"
"I do not like them\n"
"with a mouse.\n"
"I do not like them\n"
"here or there.\n"
"I do not like them\n"
"anywhere.\n"
"I do not like green eggs and ham.\n"
"I do not like them, Sam-I-am.\n"
"Would you eat them\n"
"in a box?\n"
"Would you eat them\n"
"with a fox?\n"
"Not in a box.\n"
"Not with a fox.\n"
"Not in a house.\n"
"Not with a mouse.\n"
"I would not eat them here or there.\n"
"I would not eat them anywhere.\n"
"I would not eat green eggs and ham.\n"
"I do not like them, Sam-I-am.\n"
"Would you? Could you?\n"
"in a car?\n"
"Eat them! Eat them!\n"
"Here they are.\n"
"I would not ,\n"
"could not,\n"
"in a car\n"
"You may like them.\n"
"You will see.\n"
"You may like them\n"
"in a tree?\n"
"d not in a tree.\n"
"I would not, could not in a tree.\n"
"Not in a car! You let me be.\n"
"I do not like them in a box.\n"
"I do not like them with a fox\n"
"I do not like them in a house\n"
"I do mot like them with a mouse\n"
"I do not like them here or there.\n"
"I do not like them anywhere.\n"
"I do not like green eggs and ham.\n"
"I do not like them, Sam-I-am.\n"
"A train! A train!\n"
"A train! A train!\n"
"Could you, would you\n"
"on a train?\n"
"Not on a train! Not in a tree!\n"
"Not in a car! Sam! Let me be!\n"
"I would not, could not, in a box.\n"
"I could not, would not, with a fox.\n"
"I will not eat them with a mouse\n"
"I will not eat them in a house.\n"
"I will not eat them here or there.\n"
"I will not eat them anywhere.\n"
"I do not like them, Sam-I-am.\n"
"Say!\n"
"In the dark?\n"
"Here in the dark!\n"
"Would you, could you, in the dark?\n"
"I would not, could not,\n"
"in the dark.\n"
"Would you, could you,\n"
"in the rain?\n"
"I would not, could not, in the rain.\n"
"Not in the dark. Not on a train,\n"
"Not in a car, Not in a tree.\n"
"I do not like them, Sam, you see.\n"
"Not in a house. Not in a box.\n"
"Not with a mouse. Not with a fox.\n"
"I will not eat them here or there.\n"
"I do not like them anywhere!\n"
"You do not like\n"
"green eggs and ham?\n"
"I do not\n"
"like them,\n"
"Sam-I-am.\n"
"Could you, would you,\n"
"with a goat?\n"
"I would not,\n"
"could not.\n"
"with a goat!\n"
"Would you, could you,\n"
"on a boat?\n"
"I could not, would not, on a boat.\n"
"I will not, will not, with a goat.\n"
"I will not eat them in the rain.\n"
"I will not eat them on a train.\n"
"Not in the dark! Not in a tree!\n"
"Not in a car! You let me be!\n"
"I do not like them in a box.\n"
"I do not like them with a fox.\n"
"I will not eat them in a house.\n"
"I do not like them with a mouse.\n"
"I do not like them here or there.\n"
"I do not like them ANYWHERE!\n"
"I do not like\n"
"green eggs\n"
"and ham!\n"
"I do not like them,\n"
"Sam-I-am.\n"
"You do not like them.\n"
"SO you say.\n"
"Try them! Try them!\n"
"And you may.\n"
"Try them and you may I say.\n"
"Sam!\n"
"If you will let me be,\n"
"I will try them.\n"
"You will see.\n"
"Say!\n"
"I like green eggs and ham!\n"
"I do!! I like them, Sam-I-am!\n"
"And I would eat them in a boat!\n"
"And I would eat them with a goat...\n"
"And I will eat them in the rain.\n"
"And in the dark. And on a train.\n"
"And in a car. And in a tree.\n"
"They are so good so good you see!\n"
"So I will eat them in a box.\n"
"And I will eat them with a fox.\n"
"And I will eat them in a house.\n"
"And I will eat them with a mouse.\n"
"And I will eat them here and there.\n"
"Say! I will eat them ANYWHERE!\n"
"I do so like\n"
"green eggs and ham!\n"
"Thank you!\n"
"Thank you,\n"
"Sam-I-am\n";
	uint8_t uncompressed_data[sizeof(data)];
	ft_bzero(uncompressed_data, sizeof(data));
	printf("%d\n", sizeof(data));
	uint64_t compressed_size = compress(data, sizeof(data));

	uint64_t i = 0;
	uint64_t i_uncompressed = 0;
	while(i < compressed_size)
	{
		if (data[i] == TOKEN_MARKER)
		{
			uint16_t n_lookup;
			uint16_t length;

//			printf("%d %d %d %d\n", data[i + 1], data[i + 2], data[i + 3], data[i + 4]);
			n_lookup = (((uint16_t) data[i + 1]) << 8) + data[i + 2];
			length = (((uint16_t) data[i + 3]) << 8) + data[i + 4];

			printf("<%d, %d>", n_lookup, length);

			ft_memcpy(uncompressed_data + i_uncompressed, uncompressed_data + i_uncompressed - n_lookup, length);
			i += LEN_TOKEN;
			i_uncompressed += length;
		}
		else
		{
			uncompressed_data[i_uncompressed] = data[i];
			printf("%c", data[i]);
//			printf("%c", data[i]);
			i++;
			i_uncompressed++;
		}
	}
	printf("YEAH\nYEAH\nYEAH\nYEAH\nYEAH\nYEAH\nYEAH\n");
	for (uint64_t i = 0; i < sizeof(uncompressed_data); i++)
	{
		printf("%c", uncompressed_data[i]);
	}
//	printf("%s", uncompressed_data);
}
