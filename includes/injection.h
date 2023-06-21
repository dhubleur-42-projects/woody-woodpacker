/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   injection.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:54:19 by dhubleur          #+#    #+#             */
/*   Updated: 2023/06/21 13:58:39 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INJECTION_H
# define INJECTION_H

# include <elf.h>
# include <sys/mman.h>
# include <fcntl.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>

void start_injection(char *input_name);
void inject(Elf64_Ehdr *header_ptr, Elf64_Phdr *segment_headers, Elf64_Shdr *section_headers, void *input_file_map, off_t input_file_size);
Elf64_Phdr *find_code_cave(Elf64_Ehdr *header, Elf64_Phdr *program_headers, size_t payload_size);
size_t use_code_cave(Elf64_Ehdr *header, Elf64_Phdr *code_cave_header, size_t payload_size);
Elf64_Phdr *get_segment_to_extend(Elf64_Ehdr *header, Elf64_Phdr *program_headers);
bool get_extend_size(size_t payload_length, Elf64_Ehdr *header, Elf64_Phdr *program_headers, size_t *extend_size);
void extend_and_shift(size_t payload_length, Elf64_Ehdr *header, Elf64_Phdr *program_headers, Elf64_Shdr *input_section_headers, Elf64_Phdr *to_extend);

#endif