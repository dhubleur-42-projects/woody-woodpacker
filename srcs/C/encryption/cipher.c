/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cipher.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 11:45:20 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/23 12:52:23 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include "encryption_utils.h"

size_t cipher(uint8_t *message, size_t len, uint8_t *key, uint8_t *iv, uint8_t *encrypted)
{
	if (check_cpu_support_aes())
		return (asm_cipher(message, len, key, iv, encrypted));
	else
		return (c_cipher(message, len, key, iv, encrypted));
}

size_t uncipher(uint8_t *encrypted, size_t len, uint8_t *key, uint8_t *iv, uint8_t *message, int cbc_mode)
{
	if (check_cpu_support_aes())
		return asm_uncipher(encrypted, len, key, iv, message, cbc_mode);
	else
		return c_uncipher(encrypted, len, key, iv, message, cbc_mode);
}
