/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_shifting.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 13:43:11 by dhubleur          #+#    #+#             */
/*   Updated: 2023/06/21 14:04:51 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "injection.h"

Elf64_Phdr *get_segment_to_extend(Elf64_Ehdr *header, Elf64_Phdr *program_headers) {
	for (int i = 0; i < header->e_phnum - 1; i++) {
		if (program_headers[i].p_type != PT_LOAD)
			continue;
		if (!(program_headers[i].p_flags & PF_X))
			continue;
		if (program_headers[i + 1].p_type != PT_LOAD)
			continue;
		return &program_headers[i];
	}
	return NULL;
}

bool get_extend_size(size_t payload_length, Elf64_Ehdr *header, Elf64_Phdr *program_headers, size_t *extend_size) {
	Elf64_Phdr *to_extend = get_segment_to_extend(header, program_headers);
	if (!to_extend)
	{
		fprintf(stderr, "Cannot find a segment to extend\n");
		return false;
	}
	size_t available_space = to_extend->p_offset - (to_extend->p_offset + to_extend->p_memsz);
	size_t needed_size = payload_length;
	needed_size -= available_space;
	needed_size += needed_size % to_extend->p_align;
	*extend_size = needed_size;
	return true;
}

void extend_and_shift(size_t payload_length, Elf64_Ehdr *header, Elf64_Phdr *program_headers, Elf64_Shdr *section_headers, Elf64_Phdr *to_extend) {
	size_t needed_size;
	if (!get_extend_size(payload_length, header, program_headers, &needed_size))
		return ;
	header->e_entry = to_extend->p_vaddr + to_extend->p_memsz;
	size_t new_size = to_extend->p_memsz + needed_size;
	to_extend->p_memsz = new_size;
	to_extend->p_filesz = new_size;
	for (int i = 0; i < header->e_phnum; i++) {
		if (program_headers[i].p_offset > to_extend->p_offset)
			program_headers[i].p_offset += needed_size;
	}
	for (int i = 0; i < header->e_shnum; i++) {
		if (section_headers[i].sh_offset > to_extend->p_offset)
			section_headers[i].sh_offset += needed_size;
	}
}