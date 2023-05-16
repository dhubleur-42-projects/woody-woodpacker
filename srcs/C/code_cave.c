/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code_cave.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:54:29 by dhubleur          #+#    #+#             */
/*   Updated: 2023/05/16 23:39:18 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "injection.h"

char payload[] = "\x31\xc0\x31\xdb\x31\xd2\x68\x0a\x59\x44\x4F\x68\x4F\x57\x00\x00\x89\xe7\xb8\x04\x00\x00\x00\xbb\x01\x00\x00\x00\xb9\x06\x00\x00\x00\xcd\x80";

char jmp[] = "\xe9\x00\x00\x00\x00";                      
char pusha[] = "\x57\x56\x52";
char popa[] = "\x5a\x5e\x5f";

#define CODE_SIZE (sizeof(payload)-1 + sizeof(jmp)-1 + sizeof(pusha)-1 + sizeof(popa)-1)

#define CODE_OFFSET (selected_header->p_offset + selected_header->p_memsz)

void insert_code(unsigned char *ptr)
{
    memcpy(ptr, pusha, sizeof(pusha)-1);
    ptr += sizeof(pusha)-1;
   
    memcpy(ptr, payload, sizeof(payload)-1);
    ptr += sizeof(payload)-1;
   
    memcpy(ptr, popa, sizeof(popa)-1);
    ptr += sizeof(popa)-1;
   
    memcpy(ptr, jmp, sizeof(jmp)-1);
}

void create_codecave(Elf64_Ehdr *header, Elf64_Shdr *section_headers, Elf64_Phdr *program_headers, void *output_file_map, off_t output_file_size) {
	(void)section_headers;
	(void)output_file_size;

	unsigned int last_entry = header->e_entry;

	int i = 0;
	for(; i < header->e_phnum - 1; i++)
    {
        if(program_headers[i].p_type == PT_LOAD && program_headers[i+1].p_type == PT_LOAD && program_headers[i].p_flags & PF_X)
            break;
    }

	Elf64_Phdr *selected_header = &program_headers[i];
	Elf64_Phdr *next_header = &program_headers[i+1];

	if(selected_header->p_memsz != selected_header->p_filesz || (CODE_OFFSET + CODE_SIZE) > (next_header->p_offset + selected_header->p_offset)) {
		printf("Error: not enough space\n");
		return;
	}

	printf("Found two PT_LOAD (%i and %i)\n", i, i+1);
	printf("First PT_LOAD: Start: 0x%.8lx, End: 0x%.8lx\n", selected_header->p_offset, selected_header->p_offset + selected_header->p_memsz);
	printf("Second PT_LOAD: Start: 0x%.8lx, End: 0x%.8lx\n", next_header->p_offset, next_header->p_offset + next_header->p_memsz);
	printf("Between them, there is %lu bytes (for a payload of %lu bytes)\n", next_header->p_offset - (selected_header->p_offset + selected_header->p_memsz), CODE_SIZE);

	header->e_entry = selected_header->p_vaddr + selected_header->p_memsz;
	printf("Old entry point: 0x%.8x\n", last_entry);
	printf("New entry point: 0x%.8lx\n", header->e_entry);

	int32_t jmp_adr = (int32_t)(last_entry - (header->e_entry + CODE_SIZE));
	memcpy(jmp + 1, &jmp_adr, sizeof(int32_t));
	printf("JMP address: 0x%.8x\n", jmp_adr);

	insert_code(output_file_map + CODE_OFFSET);

	selected_header->p_memsz += CODE_SIZE;
    selected_header->p_filesz += CODE_SIZE;

	printf("New PT_LOAD: Start: 0x%.8lx, End: 0x%.8lx\n", selected_header->p_offset, selected_header->p_offset + selected_header->p_memsz);
}