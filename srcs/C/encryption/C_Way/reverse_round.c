/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_round.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 15:51:57 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/19 17:18:20 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

#include "encryption_utils.h"

void	reverse_first_round(uint8_t *block, uint8_t *initial_key)
{
	xor_bytes(block, initial_key, 16);
}

void	reverse_last_round(uint8_t *block, uint8_t *key)
{
	xor_bytes(block, key, 16);
	inv_shift_rows(block);
	inv_sub_bytes(block, 16);
}

void	reverse_regular_round(uint8_t *block, uint8_t *key)
{
	xor_bytes(block, key, 16);
	inv_mix_columns(block);
	inv_shift_rows(block);
	inv_sub_bytes(block, 16);
}
