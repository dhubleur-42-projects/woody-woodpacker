/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:35:33 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/23 11:57:07 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdint.h>

size_t ft_strlen(char *str)
{
	size_t	size = 0;

	for (; str[size]; size++)
		;
	return (size);
}

int	is_valid_hex(char *hex)
{
	if (ft_strlen(hex) < 3)
		return (0);
	if (hex[0] != '0' || hex[1] != 'x')
		return (0);
	for (int i = 2; hex[i]; i++)
	{
		if (!((hex[i] >= '0' && hex[i] <= '9') || (hex[i] >= 'A' && hex[i] <= 'F')))
			return (0);
	}
	return (1);
}

void	ft_memcpy(void *dest, void *src, size_t n_bytes)
{
	uint8_t	*b_dest = (uint8_t *) dest;
	uint8_t	*b_src = (uint8_t *) src;

	for (size_t i = 0; i < n_bytes; i++)
		b_dest[i] = b_src[i];
}
