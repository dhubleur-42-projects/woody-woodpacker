/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_a_round.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 15:10:05 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/12 19:19:36 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

#include "encryption_utils.h"

// We assume block has a length of 16 bytes and key has a length of 16 bytes
void	make_a_round(uint8_t *block, uint8_t *key)
{
	sub_bytes(block, 16);
	shift_rows(block);
	mix_columns(block);
	for (int i = 0; i < 16; i++)
		block[i] ^= key[i];
}
