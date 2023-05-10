/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mix_columns.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 16:00:32 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/10 16:44:26 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

static const uint8_t MDS_MATRIX[] = {2, 3, 1, 1, 1, 2, 3, 1, 1, 1, 2, 3, 3, 1, 1, 2};

static void	multiply_matrix_4_4_and_vec_4(uint8_t const *matrix, uint8_t const *vec, uint8_t *result);

void	mix_columns(uint8_t *box)
{
	for (int i = 0; i < 4; i++)
	{
		uint8_t column[4] = {box[i], box[4 + i], box[2 * 4 + i], box[3 * 4 + i]};
		uint8_t result[4];

		multiply_matrix_4_4_and_vec_4(MDS_MATRIX, column, result);
		for (int j = 0; j < 4; j++)
			box[j * 4 + i] = result[j];
	}
}

static void	multiply_matrix_4_4_and_vec_4(uint8_t const *matrix, uint8_t const *vec, uint8_t *result)
{
	for (int i = 0; i < 4; i++)
	{
		result[i] = matrix[i * 4] * vec[0]
				+	matrix[i * 4 + 1] * vec[1]
				+	matrix[i * 4 + 2] * vec[2]
				+	matrix[i * 4 + 3] * vec[3];
	}
}
