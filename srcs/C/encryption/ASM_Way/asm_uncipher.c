/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_uncipher.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 18:47:36 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/16 20:22:33 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <wmmintrin.h>

// TODO : Key in good order ? REgarde la boucle
// Faire en sorte de ne pas tout copier le premier (donc le dernier) block dans message, celui qui contient du padding
// Vérifie que tout fonctionne avec tout plein de cas d'exemples
size_t aes_cbc_decrypt(uint8_t *encrypted, size_t len, uint8_t *key, uint8_t *iv, uint8_t *message)
{
	__m128i data, feedback, last_in;
	int i, j;
	int	n_block;
	uint8_t subkeys[240];
	uint8_t	block[16];
	size_t	len_message;

	generate_keys(key, subkeys);
	if (len % 16)
		len = len / 16 + 1;
	else
		len /= 16;
	feedback = _mm_loadu_si128((__m128i*) iv);
	for (i = 0; i < len; i++)
	{
		last_in = _mm_loadu_si128(&((__m128i*) encrypted)[i]);
		data = _mm_xor_si128(last_in, ((__m128i*) subkeys)[0]);
		for (j = 1; j < 14; j++)
			data = _mm_aesdec_si128(data, ((__m128i*) subkeys)[j]);
		data = _mm_aesdeclast_si128(data, ((__m128i*) subkeys)[j]);
		data = _mm_xor_si128(data,feedback);
		_mm_storeu_si128((__m128i *) block, data);
		if (i != len - 1)
			ft_memcpy(message + i * 16, block, 16);
		else
			kkk
//		_mm_storeu_si128(&((__m128i*) message)[i], data);
		feedback = last_in;
	}
}
