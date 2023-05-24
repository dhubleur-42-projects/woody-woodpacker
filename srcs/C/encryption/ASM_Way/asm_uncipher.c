/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_uncipher.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 18:47:36 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/24 12:27:00 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <wmmintrin.h>
#include <stdint.h>

#include "encryption_utils.h"

#include "libft.h"

size_t asm_uncipher(uint8_t *encrypted, size_t len, uint8_t *key, uint8_t *iv, uint8_t *message)
{
	__m128i data, feedback, last_in;
	size_t i, j;
	uint8_t subkeys[240];
	uint8_t	block[16];
	size_t	len_message;

	generate_keys(key, subkeys);

	feedback = _mm_loadu_si128((__m128i*) iv);
	len /= 16;
	for (i = 0; i < len; i++)
	{
		last_in = _mm_loadu_si128(&((__m128i*) encrypted)[i]);
		data = _mm_xor_si128(last_in, ((__m128i*) subkeys)[14]);
		for (j = 13; j > 0; j--)
			data = _mm_aesdec_si128(data, _mm_aesimc_si128(((__m128i*) subkeys)[j]));
		data = _mm_aesdeclast_si128(data, ((__m128i*) subkeys)[0]);
		data = _mm_xor_si128(data,feedback);
		if (i == len - 1)
		{
			_mm_storeu_si128((__m128i_u *)&block, data);
			ft_memcpy(message + i * 16, block, 16 - block[15]);
			len_message = len - block[15];
		}
		else
			_mm_storeu_si128(&((__m128i*) message)[i], data);
		feedback = last_in;
	}
	return (len_message);
}
