/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 17:35:36 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/15 18:00:49 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdbool.h>	
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

typedef struct s_options
{
	char	*seed;
	bool	compression;
	bool	help;
	bool 	verbose;
}	t_options;

typedef struct s_parser
{
	t_options	options;
	char		*input;
}	t_parser;

bool	parse(int argc, char **argv, t_parser *parser);
bool parse_options(int argc, char **argv, int *current_arg, t_options *options);

#endif