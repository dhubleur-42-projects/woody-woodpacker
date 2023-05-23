/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uncipher.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 15:58:32 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/19 17:37:02 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

#include "encryption_utils.h"

static void	uncipher_block(uint8_t *block, uint8_t *subkeys);

// Return length of message
size_t	uncipher(uint8_t *encrypted, size_t len, uint8_t *key, uint8_t *iv, uint8_t *message)
{
	uint8_t	subkeys[240];
	size_t	len_message;

	generate_keys(key, subkeys);
	for (size_t i = len / 16 - 1; i != (size_t) -1; i--)
	{
		uint8_t	*current_encrypted_block = encrypted + i * 16;
		uint8_t *current_message_block = message + i * 16;
		uint8_t *current_vec = i == 0 ? iv : encrypted + (i - 1) * 16;

		uncipher_block(current_encrypted_block, subkeys);
		xor_bytes(current_encrypted_block, current_vec, 16);
		len_message = len - encrypted[len - 1];
		if (i != len / 16 - 1)
			ft_memcpy(current_message_block, current_encrypted_block, 16);
		else
			ft_memcpy(current_message_block, current_encrypted_block, len_message % 16);
	}
	return (len_message);
}

static void	uncipher_block(uint8_t *block, uint8_t *subkeys)
{
	reverse_last_round(block, subkeys + 14 * 16);
	for (int i = 12; i >= 0; i--)
		reverse_regular_round(block, subkeys + (i + 1) * 16);
	reverse_first_round(block, subkeys);
}
