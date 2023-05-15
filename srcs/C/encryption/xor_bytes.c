/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xor_bytes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 14:43:07 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/15 14:44:12 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdlib.h>

#include "encryption_utils.h"

void	xor_bytes(uint8_t *a, uint8_t *b, size_t n_bytes)
{
	for (size_t i = 0; i < n_bytes; i++)
		a[i] ^= b[i];
}
