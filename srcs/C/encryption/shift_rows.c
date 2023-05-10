/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shift_rows.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 13:45:04 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/10 15:54:48 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdlib.h>

static void	left_shift(uint8_t *array, size_t len, size_t n_times);
static void	one_left_shift(uint8_t *array, size_t len);

void	shift_rows(uint8_t *box)
{
	for (int i = 1; i < 4; i++)
		left_shift(box + 4 * i, 4, i);
}

static void	left_shift(uint8_t *array, size_t len, size_t n_times)
{
	for (int j = 0; j < n_times; j++)
		one_left_shift(array, len);
}

static void	one_left_shift(uint8_t *array, size_t len)
{
	uint8_t const	old_first_value = array[0];
	for (int i = 0; i < len - 1; i++)
		array[i] = array[(i + 1) % len];

	array[len - 1] = old_first_value;
}
