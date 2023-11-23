/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_generator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 15:38:53 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/23 16:31:14 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "key_generator.h"

unsigned int generate_seed(char *string)
{
	unsigned int seed;

	seed = 0;
	while (*string)
	{
		seed += *string;
		string++;
	}
	return (seed);
}

static char get_random_printable_char()
{
	return (char) (ft_rand() % 94 + 33);
}

void	generate_key(unsigned int seed, char buffer[17])
{
	buffer[16] = 0;

	ft_srand(seed);
	for (int i = 0; i < 16; i++)
	{
		buffer[i] = get_random_printable_char();
	}
}

void get_key(t_options options, char buffer[17])
{
	if (options.key)
	{
		strcpy(buffer, options.key);
	}
	else
	{
		unsigned int seed = 0;
		if (options.seed)
			seed = generate_seed(options.seed);
		else
			seed = get_seed_from_timestamp();
		generate_key(seed, buffer);
	}
}