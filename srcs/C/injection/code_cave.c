/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code_cave.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:54:29 by dhubleur          #+#    #+#             */
/*   Updated: 2023/05/22 13:56:10 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "injection.h"

size_t use_code_cave(Elf64_Ehdr *header, Elf64_Phdr *code_cave_header, size_t payload_size) {
	header->e_entry = code_cave_header->p_vaddr + code_cave_header->p_memsz;
	code_cave_header->p_memsz += payload_size;
    code_cave_header->p_filesz += payload_size;
	return code_cave_header->p_offset + code_cave_header->p_memsz - payload_size;
}

Elf64_Phdr *find_code_cave(Elf64_Ehdr *header, Elf64_Phdr *program_headers, size_t payload_size) {
	for (int i = 0; i < header->e_phnum - 1; i++) {
		if (program_headers[i].p_type != PT_LOAD)
			continue;
		if (!(program_headers[i].p_flags & PF_X))
			continue;
		if (program_headers[i + 1].p_type != PT_LOAD)
			continue;
		size_t available_space = program_headers[i + 1].p_offset - (program_headers[i].p_offset + program_headers[i].p_memsz);
		if (available_space < payload_size)
			continue;
		return &program_headers[i];
	}
	return NULL;
}