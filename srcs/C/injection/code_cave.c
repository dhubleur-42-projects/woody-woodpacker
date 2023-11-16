/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code_cave.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:54:29 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/16 15:05:06 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prepare_injection.h"

size_t use_code_cave_elf64(Elf64_Ehdr *header, Elf64_Phdr *code_cave_header, size_t payload_size) {
	header->e_entry = code_cave_header->p_vaddr + code_cave_header->p_memsz;
	code_cave_header->p_memsz += payload_size;
    code_cave_header->p_filesz += payload_size;
	return code_cave_header->p_offset + code_cave_header->p_memsz - payload_size;
}

Elf64_Phdr *find_code_cave_elf64(t_file_elf64 file_elf64, size_t payload_size) {
	for (int i = 0; i < file_elf64.header->e_phnum - 1; i++) {
		if (file_elf64.programs[i].p_type != PT_LOAD)
			continue;
		if (!(file_elf64.programs[i].p_flags & PF_X))
			continue;
		if (file_elf64.programs[i + 1].p_type != PT_LOAD)
			continue;
		size_t available_space = file_elf64.programs[i + 1].p_offset - (file_elf64.programs[i].p_offset + file_elf64.programs[i].p_memsz);
		if (available_space < payload_size)
			continue;
		return &(file_elf64.programs[i]);
	}
	return NULL;
}