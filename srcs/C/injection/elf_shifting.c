/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_shifting.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 13:43:11 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/17 15:30:15 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prepare_injection.h"

size_t get_extend_size_elf64(size_t payload_length) {
	size_t extend_size = 0;
	extend_size += sizeof(Elf64_Shdr);
	extend_size += payload_length;
	return (extend_size);
}

void create_new_section_header_elf64(size_t payload_length, Elf64_Ehdr *header, Elf64_Shdr *section_headers, Elf64_Phdr *programs_headers, Elf64_Shdr *new_section_header) {
	Elf64_Shdr *last_section = &section_headers[header->e_shnum - 1];
	new_section_header->sh_name = 1;
	new_section_header->sh_type = SHT_PROGBITS;
	new_section_header->sh_flags = SHF_ALLOC | SHF_EXECINSTR | SHF_WRITE | SHF_GROUP | SHF_TLS;
	new_section_header->sh_addr = last_section->sh_addr + last_section->sh_size;
	new_section_header->sh_offset = last_section->sh_offset + last_section->sh_size;
	new_section_header->sh_size = payload_length;
	new_section_header->sh_link = 0;
	new_section_header->sh_info = 0;
	new_section_header->sh_addralign = 0;
	new_section_header->sh_entsize = 0;

	Elf64_Phdr *last_program = &programs_headers[header->e_phnum - 1];
	unsigned long current_end = last_program->p_offset + last_program->p_filesz;
	if (new_section_header->sh_offset + new_section_header->sh_size > current_end)
	{
		last_program->p_filesz += new_section_header->sh_offset + new_section_header->sh_size - current_end;
		last_program->p_memsz += new_section_header->sh_offset + new_section_header->sh_size - current_end;
	}
	last_program->p_flags |= PF_W | PF_X | PF_R;

	header->e_shnum++;
}

void make_space_for_new_section_elf64(size_t payload_length, Elf64_Ehdr *header, void *output_map, Elf64_Shdr *new_section_header, off_t old_file_size, t_options options) {
	off_t begin_new_section = new_section_header->sh_offset;
	off_t end_new_section = begin_new_section + payload_length;
	size_t size_to_move = old_file_size - begin_new_section;
	if (options.verbose)
		printf("Created a section: begin: 0x%.8lx end: 0x%.8lx\n", begin_new_section, end_new_section);
	memmove(output_map + end_new_section, output_map + begin_new_section, size_to_move);
	if (options.verbose)
		printf("Shifted %ld bytes\n", size_to_move);
	header->e_shoff += end_new_section - begin_new_section;
}

size_t extend_and_shift_elf64(size_t payload_length, t_file_elf64 file, void *output_map, off_t old_file_size, t_injection *injection, t_options options) {
	off_t diff = file.programs[0].p_vaddr - file.programs[0].p_offset;
	Elf64_Shdr *new_section_header = &(file.sections[file.header->e_shnum]);
	create_new_section_header_elf64(payload_length, file.header, file.sections, file.programs, new_section_header);
	file.header->e_entry = diff + new_section_header->sh_offset;
	injection->new_entrypoint = file.header->e_entry;
	size_t payload_offset = new_section_header->sh_offset;
	make_space_for_new_section_elf64(payload_length, file.header, output_map, new_section_header, old_file_size, options);
	return (payload_offset);
}