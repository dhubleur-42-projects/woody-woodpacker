/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code_cave.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:54:29 by dhubleur          #+#    #+#             */
/*   Updated: 2023/05/21 13:49:11 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "injection.h"

/*
bits 64
default rel
global _start

_start:
		push rax
		push rdi
		push rsi
		push rdx
        xor     eax, eax
        cdq
        mov     dl, 10
        inc     eax
        mov     edi, eax
        lea     rsi, [rel msg]
        syscall
		pop rdx
		pop rsi
		pop rdi
		pop rax
		jmp 0x42424242

msg     db "..WOODY..",10
*/

char payload_part_1[] = "\x31\xc0\x99\xb2\x0a\xff\xc0\x89\xc7\x48\x8d\x35\x0b\x00\x00\x00\x0f\x05";
char payload_part_2[] = "\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x0a";

// jmp 0x00000000 (to be replaced)
char jmp[] = "\xe9\x00\x00\x00\x00";  

// push rax; push rdi; push rsi; push rdx
char save_registers[] = "\x50\x57\x56\x52";

// pop rdx; pop rsi; pop rdi; pop rax
char load_registers[] = "\x5a\x5e\x5f\x58";

#define PAYLOAD_LENGTH (sizeof(payload_part_1)-1 + sizeof(payload_part_2)-1 + sizeof(jmp)-1 + sizeof(save_registers)-1 + sizeof(load_registers)-1)
#define PAYLOAD_TO_JMP_LENGTH (sizeof(payload_part_1)-1 + sizeof(save_registers)-1 + sizeof(load_registers)-1 + sizeof(jmp)-1)

void insert_code(unsigned char *ptr)
{	
    memcpy(ptr, save_registers, sizeof(save_registers)-1);
    ptr += sizeof(save_registers)-1;
   
    memcpy(ptr, payload_part_1, sizeof(payload_part_1)-1);
    ptr += sizeof(payload_part_1)-1;
   
    memcpy(ptr, load_registers, sizeof(load_registers)-1);
    ptr += sizeof(load_registers)-1;

    memcpy(ptr, jmp, sizeof(jmp)-1);
	ptr += sizeof(jmp)-1;

	memcpy(ptr, payload_part_2, sizeof(payload_part_2)-1);
    ptr += sizeof(payload_part_2)-1;
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

	Elf64_Off payload_offset = selected_header->p_offset + selected_header->p_memsz;

	if(selected_header->p_memsz != selected_header->p_filesz || (payload_offset + PAYLOAD_LENGTH) > (next_header->p_offset + selected_header->p_offset)) {
		printf("Error: not enough space\n");
		return;
	}

	printf("Found two PT_LOAD (%i and %i)\n", i, i+1);
	printf("First PT_LOAD: Start: 0x%.8lx, End: 0x%.8lx\n", selected_header->p_offset, selected_header->p_offset + selected_header->p_memsz);
	printf("Second PT_LOAD: Start: 0x%.8lx, End: 0x%.8lx\n", next_header->p_offset, next_header->p_offset + next_header->p_memsz);
	printf("Between them, there is %lu bytes (for a payload of %lu bytes)\n", next_header->p_offset - (selected_header->p_offset + selected_header->p_memsz), PAYLOAD_LENGTH);

	header->e_entry = selected_header->p_vaddr + selected_header->p_memsz;
	printf("Old entry point: 0x%.8x\n", last_entry);
	printf("New entry point: 0x%.8lx\n", header->e_entry);

	int32_t jmp_adr = (int32_t)(last_entry - (header->e_entry + PAYLOAD_TO_JMP_LENGTH));
	memcpy(jmp + 1, &jmp_adr, sizeof(int32_t));
	printf("JMP address: 0x%.8x\n", jmp_adr);

	insert_code(output_file_map + payload_offset);

	selected_header->p_memsz += PAYLOAD_LENGTH;
    selected_header->p_filesz += PAYLOAD_LENGTH;

	printf("New PT_LOAD: Start: 0x%.8lx, End: 0x%.8lx\n", selected_header->p_offset, selected_header->p_offset + selected_header->p_memsz);
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
		printf("Found code cave of %lu bytes (for a payload of %lu bytes) between PT_LOAD %i and %i\n", available_space, payload_size, i, i + 1);
		return &program_headers[i];
	}
	printf("Impossible to found a code cave\n");
	return NULL;
}

Elf64_Phdr *create_segment(Elf64_Ehdr *header, Elf64_Phdr *program_headers, size_t payload_size) {
	bool found = false;
	int i = 0;
	for(; i < header->e_phnum; i++)
	{
		if (program_headers[i].p_type != PT_LOAD)
			continue;
		if (i == header->e_phnum - 1)
		{
			found = true;
			break;
		}
		if (program_headers[i + 1].p_type != PT_LOAD)
		{
			found = true;
			break;
		}
	}
	if (!found)
	{
		printf("Impossible to find the last PT_LOAD segment\n");
		return NULL;
	}
	printf("Found the last PT_LOAD segment at index %i (from 0x%.8lx to 0x%.8lx)\n", i, program_headers[i].p_offset, program_headers[i].p_offset + program_headers[i].p_memsz);
	size_t space_align_segment = program_headers[i].p_align - (program_headers[i].p_offset + program_headers[i].p_memsz) % program_headers[i].p_align;
	printf("Space to align segment: %lu (aligment page: %lu)\n", space_align_segment, program_headers[i].p_align);
	program_headers = realloc(program_headers, sizeof(Elf64_Phdr) * (header->e_phnum + 1));
	for (int j = header->e_phnum - 1; j > i; j--)
		program_headers[j + 1] = program_headers[j];
	Elf64_Phdr *new_segment = &program_headers[i + 1];
	new_segment->p_type = PT_LOAD;
	new_segment->p_flags = PF_R | PF_X;
	new_segment->p_offset = program_headers[i].p_offset + program_headers[i].p_memsz + space_align_segment;
	new_segment->p_vaddr = program_headers[i].p_vaddr + program_headers[i].p_memsz + space_align_segment;
	new_segment->p_paddr = program_headers[i].p_paddr + program_headers[i].p_memsz + space_align_segment;
	new_segment->p_filesz = payload_size;
	new_segment->p_memsz = payload_size;
	new_segment->p_align = program_headers[i].p_align;
	printf("New segment created at index %i (from 0x%.8lx to 0x%.8lx)\n", i + 1, new_segment->p_offset, new_segment->p_offset + new_segment->p_memsz);
	return new_segment;
}