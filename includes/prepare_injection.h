/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_injection.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 14:53:20 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/23 16:26:08 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PREPARE_INJECTION_H
# define PREPARE_INJECTION_H

# include <elf.h>
# include <sys/mman.h>
# include <fcntl.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>

# include "parser.h"
# include "libft.h"

typedef struct s_injection
{
	int 	fd;
	char	*file_map;
	off_t	file_size;
	off_t	encrypt_offset;
	int		encrypt_size;
	size_t	old_entrypoint;
	size_t 	new_entrypoint;
	off_t	payload_offset;
}	t_injection;

bool	prepare_injection(t_file file, t_injection *injection, t_options options);
bool	prepare_injection_elf64(t_file file, t_injection *injection, t_options options);
void end_injection(t_injection injection);

Elf64_Phdr *find_code_cave_elf64(t_file_elf64 file_elf64, size_t payload_size);
size_t use_code_cave_elf64(Elf64_Ehdr *header, Elf64_Phdr *code_cave_header, size_t payload_size, t_injection *injection);
size_t get_extend_size_elf64(size_t payload_length);
size_t extend_and_shift_elf64(size_t payload_length, t_file_elf64 file, void *output_map, off_t old_file_size, t_injection *injection, t_options options);
size_t get_payload_length();

#endif