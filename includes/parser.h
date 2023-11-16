/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 17:35:36 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/16 16:57:58 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdbool.h>	
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/mman.h>
# include <elf.h>
# include <unistd.h>

# include "elf_parser.h"

# define USAGE_ERROR "usage: ./woody_woodpacker <options> [file]\n"

typedef struct s_options
{
	char	*seed;
	char	*key;
	bool	compression;
	bool	help;
	bool 	verbose;
}	t_options;

typedef struct s_parser
{
	t_options	options;
	char		*input;
}	t_parser;

typedef enum e_file_type
{
	NONE,
	ELF64,
	ELF32,
}	t_file_type;

typedef struct s_file_elf64
{
	Elf64_Ehdr	*header;
	Elf64_Shdr	*sections;
	Elf64_Phdr	*programs;
}	t_file_elf64;

typedef struct s_file
{
	t_file_type	type;
	int			fd;
	void		*map;
	off_t		size;
	void		*specific_file;
}	t_file;

bool	parse(int argc, char **argv, t_parser *parser);
bool parse_options(int argc, char **argv, int *current_arg, t_options *options);
bool	get_file_type(char *name, t_file *file, t_options options);
void close_file(t_file file);
bool get_specific_file(t_file *file, t_options options);

#endif