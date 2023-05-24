/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_cipher.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 18:34:04 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/23 12:11:30 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <wmmintrin.h>
#include <stdint.h>

#include "encryption_utils.h"
#include "libft.h"
#include <strings.h>

size_t asm_cipher(uint8_t *message, size_t len, uint8_t *key, uint8_t *iv, uint8_t *encrypted)
{
	__m128i feedback, data;
	int i, j;
	int	n_block;
	uint8_t subkeys[240];
	uint8_t	block[16];

	generate_keys(key, subkeys);
	n_block = len / 16 + 1;
	feedback = _mm_loadu_si128((__m128i*) iv);
	for (i = 0; i < n_block; i++)
	{
		if (i != n_block - 1)
			ft_memcpy(block, message + i * 16, 16);
		else
		{
			ft_memcpy(block, message + i * 16, len % 16);
			add_padding(block, len % 16);
		}
		data = _mm_loadu_si128((__m128i*) block);
		feedback = _mm_xor_si128(data, feedback);
		feedback = _mm_xor_si128(feedback, ((__m128i*) subkeys)[0]);
		for(j = 1; j < 14; j++)
			feedback = _mm_aesenc_si128(feedback, ((__m128i*) subkeys)[j]);
		feedback = _mm_aesenclast_si128(feedback, ((__m128i*) subkeys)[j]);
		_mm_storeu_si128(&((__m128i*) encrypted)[i], feedback);
	}
	return (n_block * 16);
}
