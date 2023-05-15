/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 14:50:38 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/15 14:52:15 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdlib.h>

#include "encryption_utils.h"

void	ft_memcpy(void *dest, void *src, size_t n_bytes)
{
	uint8_t	*b_dest = (uint8_t *) dest;
	uint8_t	*b_src = (uint8_t *) src;

	for (size_t i = 0; i < n_bytes; i++)
		b_dest[i] = b_src[i];
}
