/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_options.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 17:42:00 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/15 17:49:09 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool parse_seed(int argc, char **argv, int *current_arg, t_options *options)
{
	if (*current_arg + 1 >= argc) {
		printf("Error: missing seed\n");
		return (false);
	}
	options->seed = argv[++(*current_arg)];
	return (true);
}

bool parse_options(int argc, char **argv, int *current_arg, t_options *options)
{
	char option = argv[*current_arg][1];

	switch (option)
	{
		case 'h':
			options->help = true;
			break;
		case 'c':
			options->compression = true;
			break;
		case 's':
			if (!parse_seed(argc, argv, current_arg, options))
				return (false);
			break;
		default:
			printf("Error: unknown option %c\n", option);
			return (false);
	}
	return (true);
}