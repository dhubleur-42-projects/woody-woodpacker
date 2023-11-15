/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_key.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:33:25 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/16 14:09:40 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include "get_key.h"

#include "libft.h"
#include "random.h"

static int	from_hex_str_to_bytes(char *hex, uint8_t *bytes);
static void	get_key_from_random(uint8_t *key);
static int	from_hex_to_byte(char hex);

// Returns 0 if success, 1 if failure
int	get_key(uint8_t *key, int ac, char **av)
{
	if (ac >= 3)
		return (from_hex_str_to_bytes(av[2], key));
	else
		get_key_from_random(key);
	return (0);
}

// Returns 0 if success, 1 if failure
static int	from_hex_str_to_bytes(char *hex, uint8_t *bytes)
{

	int	i = 2;

	if (!is_valid_hex(hex))
		return (1);
	for (int j = 0; hex[i]; j++, i += 2)
		bytes[j] = from_hex_to_byte(hex[i]) * 16 + from_hex_to_byte(hex[i + 1]);
	return (0);
}

// Returns -1 on error
static int	from_hex_to_byte(char hex)
{
	static const char *HEX_CHARS = "0123456789ABCDEF";

	for (int i = 0; HEX_CHARS[i]; i++)
		if (HEX_CHARS[i] == hex)
			return (i);
	return (-1);
}

static void	get_key_from_random(uint8_t *key)
{
	for (int i = 0; i < 32; i++)
		key[i] = ft_rand() & 0xFF;
}
