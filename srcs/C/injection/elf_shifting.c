/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_shifting.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 13:43:11 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/20 18:05:37 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prepare_injection.h"

size_t get_extend_size_elf64(size_t payload_length, t_file_elf64 file_elf64) {
	for (int i = 0; i < file_elf64.header->e_phnum - 1; i++) {
		if (file_elf64.programs[i].p_type != PT_LOAD)
			continue;
		if (!(file_elf64.programs[i].p_flags & PF_X))
			continue;
		if (file_elf64.programs[i + 1].p_type != PT_LOAD)
			continue;
		off_t last_address_used = file_elf64.programs[i].p_offset + file_elf64.programs[i].p_memsz + payload_length;
		size_t bytes_used_to_align = file_elf64.programs[i + 1].p_align - last_address_used % file_elf64.programs[i + 1].p_align;
		off_t new_offset = last_address_used + bytes_used_to_align;
		off_t shifting = new_offset - file_elf64.programs[i + 1].p_offset;
		return (shifting);
	}
	return (0);
}

void	extend_and_shift_elf64(size_t payload_length, char *map, size_t map_length, t_options options) {
	t_file_elf64 file_elf64;

	file_elf64.header = (Elf64_Ehdr *)map;
	file_elf64.programs = (Elf64_Phdr *)(map + file_elf64.header->e_phoff);
	file_elf64.sections = (Elf64_Shdr *)(map + file_elf64.header->e_shoff);

	for (int i = 0; i < file_elf64.header->e_phnum - 1; i++) {
		if (file_elf64.programs[i].p_type != PT_LOAD)
			continue;
		if (!(file_elf64.programs[i].p_flags & PF_X))
			continue;
		if (file_elf64.programs[i + 1].p_type != PT_LOAD)
			continue;

		if (options.verbose)
			printf("Extend segment at 0x%.8lx and use it as a code cave\n", file_elf64.programs[i].p_offset);

		off_t last_address_used = file_elf64.programs[i].p_offset + file_elf64.programs[i].p_memsz + payload_length;
		size_t bytes_used_to_align = file_elf64.programs[i + 1].p_align - last_address_used % file_elf64.programs[i + 1].p_align;
		off_t new_offset = last_address_used + bytes_used_to_align;
		unsigned long old_offset = file_elf64.programs[i + 1].p_offset;
		off_t shifting = new_offset - old_offset;
		if (options.verbose)
			printf("Shifting %ld bytes with an offset of %ld\n", map_length - old_offset, shifting);

		for (int j = 0; j < file_elf64.header->e_shnum; j++) {
			if (file_elf64.sections[j].sh_offset >= old_offset)
				file_elf64.sections[j].sh_offset += shifting;
		}
		for (int j = 0; j < file_elf64.header->e_phnum; j++) {
			if (file_elf64.programs[j].p_offset >= old_offset)
				file_elf64.programs[j].p_offset += shifting;
		}
		file_elf64.header->e_shoff += shifting;
		memmove(map + new_offset, map + old_offset, map_length - old_offset);
		bzero(map + old_offset, shifting);
		
		if (options.verbose)
		{
			file_elf64.programs = (Elf64_Phdr *)(map + file_elf64.header->e_phoff);
			size_t available_space = file_elf64.programs[i + 1].p_offset - (file_elf64.programs[i].p_offset + file_elf64.programs[i].p_memsz);
			printf("New available space: %lu (between 0x%.8lx and 0x%.8lx)\n", available_space, file_elf64.programs[i].p_offset + file_elf64.programs[i].p_memsz, file_elf64.programs[i + 1].p_offset);
		}
	}
}