/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 17:37:30 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/15 17:49:29 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	parse(int argc, char **argv, t_parser *parser)
{
	parser->options = (t_options) {NULL, false, false};
	parser->input = NULL;

	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			if (strlen(argv[i]) != 2) {
				printf("usage: ./woody_woodpacker <options> [file]\n");
				return (false);
			}
			if (!parse_options(argc, argv, &i, &parser->options))
				return (false);
		}
		else
		{
			if (parser->input == NULL) {
				parser->input = argv[i];
			} else {
				printf("usage: ./woody_woodpacker <options> [file]\n");
				return (false);
			}
		}
	}

	return true;
}