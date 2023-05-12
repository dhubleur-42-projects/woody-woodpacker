/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_keys.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 14:23:19 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/12 15:06:40 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

#include "encryption_utils.h"

#define KEY_LENGTH	8
#define N_ROUNDS	15

static const uint32_t RCONS[] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000};

// We assume initial_key has a length of 8 32-bits words and subkeys has a length of
// (15 + 1) = 16 32-bits words (because of aes-256)
void	generate_keys(uint32_t *initial_key, uint32_t *subkeys)
{
	for (int i = 0; i < 4 * N_ROUNDS; i++)
	{
		if (i < KEY_LENGTH)
			subkeys[i] = initial_key[i];
		else if (i % KEY_LENGTH == 0)
		{
			uint32_t transformed_previous_word = subkeys[i - 1];

			word_left_shift(&transformed_previous_word);
			sub_word(&transformed_previous_word);
			subkeys[i] = subkeys[i - KEY_LENGTH] ^ transformed_previous_word;
			subkeys[i] ^= RCONS[i / KEY_LENGTH - 1];
		}
		else if (i % KEY_LENGTH == 4)
		{
			uint32_t transformed_previous_word = subkeys[i - 1];

			sub_word(&transformed_previous_word);
			subkeys[i] = subkeys[i - KEY_LENGTH] ^ transformed_previous_word;
		}
		else
			subkeys[i] = subkeys[i - KEY_LENGTH] ^ subkeys[i - 1];
	}
}
