/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mix_columns.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 16:00:32 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/12 19:17:49 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

static const uint8_t MDS_MATRIX[] = {2, 3, 1, 1, 1, 2, 3, 1, 1, 1, 2, 3, 3, 1, 1, 2};

static void	multiply_matrix_4_4_and_vec_4(uint8_t const *matrix, uint8_t const *vec, uint8_t *result);
static uint8_t bit_multiply(uint8_t matrix_nbr, uint8_t vec_nbr);

// We assume box is a 4 x 4 matrix, stored column by column in a array
void	mix_columns(uint8_t *box)
{
	for (int i = 0; i < 4; i++)
	{
		uint8_t result[4];

		multiply_matrix_4_4_and_vec_4(MDS_MATRIX, box + i * 4, result);
		for (int j = 0; j < 4; j++)
			box[i * 4 + j] = result[j];
	}
}

static void	multiply_matrix_4_4_and_vec_4(uint8_t const *matrix, uint8_t const *vec, uint8_t *result)
{
	for (int i = 0; i < 4; i++)
	{
		result[i] = bit_multiply(matrix[i * 4], vec[0])
				^	bit_multiply(matrix[i * 4 + 1], vec[1])
				^	bit_multiply(matrix[i * 4 + 2], vec[2])
				^	bit_multiply(matrix[i * 4 + 3], vec[3]);
	}
}

static uint8_t bit_multiply(uint8_t matrix_nbr, uint8_t vec_nbr)
{
	switch (matrix_nbr)
	{
		case 1:
			return vec_nbr;
		case 2:
			uint8_t ret = vec_nbr << 1;
			if (vec_nbr & 0x80)
				ret ^= 0x1b;
			return ret;
		case 3:
			return bit_multiply(2, vec_nbr) ^ vec_nbr;
		default:
			return -42;
	}
}
