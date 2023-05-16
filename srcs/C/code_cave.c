/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code_cave.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:54:29 by dhubleur          #+#    #+#             */
/*   Updated: 2023/05/16 16:46:40 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "injection.h"

void create_codecave(Elf64_Ehdr *header, Elf64_Shdr *section_headers, Elf64_Phdr *program_headers, void *output_file_map, off_t output_file_size, char payload[], off_t payload_len) {
    (void)header;
	(void)section_headers;
	(void)program_headers;
	(void)output_file_map;
	(void)output_file_size;
	(void)payload;
	(void)payload_len;

	void *payload_place = section_headers;

	memmove(section_headers + payload_len, section_headers, header->e_shentsize * header->e_shnum);
	section_headers += payload_len;
	header->e_shoff += payload_len;
	for (int i = 0; i < header->e_shnum; i++) {
		section_headers[i].sh_offset += payload_len;
	}

	void *new_section_place = output_file_map + output_file_size - header->e_shentsize;

	Elf64_Shdr *new_section = new_section_place;
	new_section->sh_name = 0;
	new_section->sh_type = SHT_PROGBITS;
	new_section->sh_flags = SHF_ALLOC | SHF_EXECINSTR;
	new_section->sh_addr = 0;
	new_section->sh_offset = new_section_place - output_file_map;
	new_section->sh_size = payload_len;
	new_section->sh_link = 0;
	new_section->sh_info = 0;
	new_section->sh_addralign = 16;
	new_section->sh_entsize = 0;
	header->e_shnum++;

	memcpy(payload_place, payload, payload_len);

	header->e_entry = payload_place - output_file_map;	
}