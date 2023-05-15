/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cipher.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 13:34:17 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/15 15:21:39 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

#include "encryption_utils.h"

static void	cipher_block(uint8_t *block, uint8_t *subkeys);
static void add_padding(uint8_t *block, size_t block_len);

// We assume key is 32 bytes long and iv is 16 bytes long
void	cipher(uint8_t *message, size_t len, uint8_t *key, uint8_t *iv, uint8_t *encrypted)
{
	uint8_t	subkeys[240];

	generate_keys(key, subkeys);
	for (size_t i = 0; i < len / 16 + 1; i++)
	{
		uint8_t	*current_encrypted_block = encrypted + i * 16;
		uint8_t *current_message_block = message + i * 16;
		uint8_t *current_vec = i == 0 ? iv : encrypted + (i - 1) * 16;

		if (i != len / 16)
			ft_memcpy(current_encrypted_block, current_message_block, 16);
		else
		{
			ft_memcpy(current_encrypted_block, current_message_block, len % 16);
			add_padding(current_encrypted_block, len % 16);
		}

		xor_bytes(current_encrypted_block, current_vec, 16);
		cipher_block(current_encrypted_block, subkeys);
	}
}

static void	cipher_block(uint8_t *block, uint8_t *subkeys)
{
	do_first_round(block, subkeys);
	for (int i = 0; i < 13; i++)
		do_regular_round(block, subkeys + (i + 1) * 16);
	do_last_round(block, subkeys + 14 * 16);
}

static void add_padding(uint8_t *block, size_t block_len)
{
	size_t	len_pad = 16 - block_len;

	for (size_t i = block_len; i < 16; i++)
		block[i] = len_pad;
}
