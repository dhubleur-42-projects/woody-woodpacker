/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_parser.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 13:56:36 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/24 16:54:51 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ELF_PARSER_H
# define ELF_PARSER_H

# include <elf.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdlib.h>

# include "parser.h"

typedef struct s_file t_file;
typedef struct s_options t_options;

bool parse_elf64(int fd, t_file *file, t_options options);
bool parse_elf32(int fd, t_file *file, t_options options);

#endif