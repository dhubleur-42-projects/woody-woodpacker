/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_a_round.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 15:10:05 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/15 17:08:34 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

#include "encryption_utils.h"

void	do_first_round(uint8_t *block, uint8_t *initial_key)
{
	xor_bytes(block, initial_key, 16);
}

void	do_last_round(uint8_t *block, uint8_t *key)
{
	sub_bytes(block, 16);
	shift_rows(block);
	xor_bytes(block, key, 16);
}

// We assume block has a length of 16 bytes and key has a length of 16 bytes
void	do_regular_round(uint8_t *block, uint8_t *key)
{
	sub_bytes(block, 16);
	shift_rows(block);
	mix_columns(block);
	xor_bytes(block, key, 16);
}
