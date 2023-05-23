/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_keys.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 14:23:19 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/12 18:05:08 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

#include "encryption_utils.h"

#define KEY_LENGTH	8
#define N_ROUNDS	15

static const uint32_t RCONS[] = {0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040};

// We assume initial_key has a length of 8 32-bits words and subkeys has a length of
// 15 * 4 = 60 32-bits words (because of aes-256)
void	generate_keys(uint8_t *b_initial_key, uint8_t *b_subkeys)
{
	uint32_t	*w_initial_key = (uint32_t *) b_initial_key;
	uint32_t	*w_subkeys = (uint32_t *) b_subkeys;

	for (int i = 0; i < 4 * N_ROUNDS; i++)
	{
		if (i < KEY_LENGTH)
			w_subkeys[i] = w_initial_key[i];
		else if (i % KEY_LENGTH == 0)
		{
			uint32_t transformed_previous_word = w_subkeys[i - 1];

			left_shift((uint8_t *) &transformed_previous_word, 4);
			sub_word(&transformed_previous_word);
			w_subkeys[i] = w_subkeys[i - KEY_LENGTH] ^ transformed_previous_word;
			w_subkeys[i] ^= RCONS[i / KEY_LENGTH - 1];
		}
		else if (i % KEY_LENGTH == 4)
		{
			uint32_t transformed_previous_word = w_subkeys[i - 1];

			sub_word(&transformed_previous_word);
			w_subkeys[i] = w_subkeys[i - KEY_LENGTH] ^ transformed_previous_word;
		}
		else
			w_subkeys[i] = w_subkeys[i - KEY_LENGTH] ^ w_subkeys[i - 1];
	}
}
