/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_options.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 17:42:00 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/15 18:04:35 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool parse_key(int argc, char **argv, int *current_arg, t_options *options)
{
	if (options->seed != NULL) {
		printf("Error: cannot precise a seed and a key\n");
		return (false);
	}
	if (*current_arg + 1 >= argc) {
		printf("Error: missing seed\n");
		return (false);
	}
	options->key = argv[++(*current_arg)];
	if (strlen(options->key) != 16) {
		printf("Error: key must be 16 characters long\n");
		return (false);
	}
	for (int i = 0; options->key[i]; i++) {
		if (options->key[i] < 32 || options->key[i] > 126) {
			printf("Error: key must be a string of printable characters\n");
			return (false);
		}
	}
	return (true);
}

bool parse_seed(int argc, char **argv, int *current_arg, t_options *options)
{
	if (options->key != NULL) {
		printf("Error: cannot precise a seed and a key\n");
		return (false);
	}
	if (*current_arg + 1 >= argc) {
		printf("Error: missing seed\n");
		return (false);
	}
	options->seed = argv[++(*current_arg)];
	for (int i = 0; options->seed[i]; i++) {
		if (options->seed[i] < 32 || options->seed[i] > 126) {
			printf("Error: seed must be a string of printable characters\n");
			return (false);
		}
	}
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
		case 'v':
			options->verbose = true;
			break;
		case 's':
			if (!parse_seed(argc, argv, current_arg, options))
				return (false);
			break;
		case 'k':
			if (!parse_key(argc, argv, current_arg, options))
				return (false);
			break;
		default:
			printf("Error: unknown option %c\n", option);
			return (false);
	}
	return (true);
}