/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shift_rows.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 13:45:04 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/12 15:05:50 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdlib.h>

#include "encryption_utils.h"

void	shift_rows(uint8_t *box)
{
	for (int i = 1; i < 4; i++)
		n_left_shift(box + 4 * i, 4, i);
}

void	n_left_shift(uint8_t *array, size_t len, size_t n_times)
{
	for (size_t j = 0; j < n_times; j++)
		left_shift(array, len);
}

void	left_shift(uint8_t *array, size_t len)
{
	uint8_t const	old_first_value = array[0];

	for (size_t i = 0; i < len - 1; i++)
		array[i] = array[(i + 1) % len];

	array[len - 1] = old_first_value;
}

// TODO: Make it endian independant ?
void word_left_shift(uint32_t *word)
{
	if (am_i_little_endian())
		right_shift((uint8_t *) word, 4);
	else
		left_shift((uint8_t *) word, 4);
}

int am_i_little_endian(void)
{
	uint16_t nbr = 1;
	uint8_t pouet = *((uint8_t *) &nbr);
	return (pouet);
}

void	right_shift(uint8_t *array, size_t len)
{
	uint8_t const	old_last_value = array[len - 1];

	for (size_t i = len - 1; i > 0; i--)
		array[i] = array[i - 1];

	array[0] = old_last_value;
}
