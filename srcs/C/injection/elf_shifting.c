/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_shifting.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 13:43:11 by dhubleur          #+#    #+#             */
/*   Updated: 2023/06/22 14:28:24 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "injection.h"

bool get_extend_size(size_t payload_length, Elf64_Ehdr *header, Elf64_Shdr *section_headers, size_t *extend_size) {
	(void)section_headers;
	(void)header;
	*extend_size = 0;
	*extend_size += sizeof(Elf64_Shdr);
	*extend_size += payload_length;
	return (true);
}

void create_new_section_header(size_t payload_length, Elf64_Ehdr *header, Elf64_Shdr *section_headers, Elf64_Shdr *new_section_header, void *output_map) {
	(void)section_headers;
	(void)output_map;
	(void)header;
	Elf64_Shdr *last_section = &section_headers[header->e_shnum - 1];
	new_section_header->sh_name = 0;
	new_section_header->sh_type = SHT_PROGBITS;
	new_section_header->sh_flags = SHF_ALLOC | SHF_EXECINSTR;
	new_section_header->sh_addr = last_section->sh_addr + last_section->sh_size;
	new_section_header->sh_offset = last_section->sh_offset + last_section->sh_size;
	new_section_header->sh_size = payload_length;
	new_section_header->sh_link = 0;
	new_section_header->sh_info = 0;
	new_section_header->sh_addralign = 0;
	new_section_header->sh_entsize = 0;

	header->e_shnum++;
}

void make_space_for_new_section(size_t payload_length, Elf64_Ehdr *header, Elf64_Shdr *section_headers, void *output_map, size_t output_file_size, Elf64_Shdr *new_section_header, off_t old_file_size) {
	(void)section_headers;
	(void)output_file_size;
	off_t begin_new_section = new_section_header->sh_offset;
	off_t end_new_section = begin_new_section + payload_length;
	size_t size_to_move = old_file_size - begin_new_section;
	printf("Begin new section: 0x%.8lx\n", begin_new_section);
	printf("End new section: 0x%.8lx\n", end_new_section);
	printf("Size to shift: %ld\n", size_to_move);
	memmove(output_map + end_new_section, output_map + begin_new_section, size_to_move);
	header->e_shoff += end_new_section - new_section_header->sh_offset;
}

size_t extend_and_shift(size_t payload_length, Elf64_Ehdr *header, Elf64_Phdr *program_headers, Elf64_Shdr *section_headers, void *output_map, size_t output_file_size, off_t old_file_size) {
	(void)output_file_size;
	(void)output_map;
	off_t diff = program_headers[0].p_vaddr - program_headers[0].p_offset;
	Elf64_Shdr *new_section_header = &section_headers[header->e_shnum];
	create_new_section_header(payload_length, header, section_headers, new_section_header, output_map);
	make_space_for_new_section(payload_length, header, section_headers, output_map, output_file_size, new_section_header, old_file_size);
	header->e_entry = diff + new_section_header->sh_offset;
	return (new_section_header->sh_offset);
}