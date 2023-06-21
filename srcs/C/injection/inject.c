/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inject.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 13:07:13 by dhubleur          #+#    #+#             */
/*   Updated: 2023/06/21 14:05:22 by dhubleur         ###   ########.fr       */
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
		jmp 0x00000000

msg     db "..WOODY..",10
*/

char payload[] = "\x50\x57\x56\x52\x31\xc0\x99\xb2\x0a\xff\xc0\x89\xc7\x48\x8d\x35\x0b\x00\x00\x00\x0f\x05\x5a\x5e\x5f\x58";
char text[] = "\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x0a";
char jmp[] = "\xe9\x00\x00\x00\x00";

#define PAYLOAD_LENGTH (sizeof(payload)-1 + sizeof(text)-1 + sizeof(jmp)-1)

void insert_payload(unsigned char *ptr, unsigned int last_entry, unsigned int current_entry)
{	
    memcpy(ptr, payload, sizeof(payload)-1);
    ptr += sizeof(payload)-1;

	printf("Old entry point: 0x%.8x\n", last_entry);
	int32_t jmp_adr = (int32_t)(last_entry - (current_entry + sizeof(payload)-1 + sizeof(jmp)-1));
	printf("Computed jump: %d\n", jmp_adr);
	memcpy(jmp + 1, &jmp_adr, sizeof(int32_t));
    memcpy(ptr, jmp, sizeof(jmp)-1);
	ptr += sizeof(jmp)-1;

	memcpy(ptr, text, sizeof(text)-1);
    ptr += sizeof(text)-1;
}

void make_injection(Elf64_Ehdr *header, Elf64_Shdr *section_headers, Elf64_Phdr *segment_headers, void *file_map) {
	// Elf64_Phdr *code_cave = find_code_cave(header, segment_headers, PAYLOAD_LENGTH);
	// if (code_cave) {
	// 	printf("Code cave segment: start: 0x%.8lx, end: 0x%.8lx\n", code_cave->p_offset, code_cave->p_offset + code_cave->p_memsz);
	// 	unsigned int last_entry = header->e_entry;
	// 	size_t injection_offset = use_code_cave(header, code_cave, PAYLOAD_LENGTH);
	// 	printf("Code cave header modified, new end: 0x%.8lx\n", code_cave->p_offset + code_cave->p_memsz);
	// 	printf("New entry point: 0x%.8lx\n", header->e_entry);
	// 	insert_payload(file_map + injection_offset, last_entry, header->e_entry);
	// 	printf("Payload injected\n");
	// } else {
		Elf64_Phdr *segment = get_segment_to_extend(header, segment_headers);
		size_t injection_offset = segment->p_offset + segment->p_memsz;
		printf("No code cave found, extending segment: start: 0x%.8lx, end: 0x%.8lx\n", segment->p_offset, segment->p_offset + segment->p_memsz);
		unsigned int last_entry = header->e_entry;
		extend_and_shift(PAYLOAD_LENGTH, header, segment_headers, section_headers, segment);
		printf("New entry point: 0x%.8lx\n", header->e_entry);
		printf("Segment extended, new end: 0x%.8lx\n", segment->p_offset + segment->p_memsz);
		printf("Start injection at 0x%.8lx\n", injection_offset);
		insert_payload(file_map + injection_offset, last_entry, header->e_entry);
		printf("Payload injected\n");
	// }
}

void inject(Elf64_Ehdr *input_header, Elf64_Phdr *input_segment_headers, Elf64_Shdr *input_section_headers, void *input_file_map, off_t input_file_size) {
	(void)input_section_headers;
	off_t output_file_size = input_file_size;
	// if (find_code_cave(input_header, input_segment_headers, PAYLOAD_LENGTH) == NULL)
	// {
		size_t needed_size;
		if (!get_extend_size(PAYLOAD_LENGTH, input_header, input_segment_headers, &needed_size))
			return ;
		output_file_size += needed_size;
	// }
	int output_fd = open("woody", O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (output_fd == -1)
	{
		perror("Cannot open file");
		return ;
	}	
	if (ftruncate(output_fd, output_file_size) == -1) {
		perror("Cannot truncate file");
		close(output_fd);
		return ;
	}
	void *output_file_map = mmap(NULL, output_file_size, PROT_WRITE, MAP_SHARED, output_fd, 0);
	if (output_file_map == MAP_FAILED) {
		perror("Cannot map output file");
		close(output_fd);
		return ;
	}
	memcpy(output_file_map, input_file_map, input_file_size);
	Elf64_Ehdr *header = output_file_map;
	Elf64_Shdr *section_headers = output_file_map + header->e_shoff;
	Elf64_Phdr *segment_headers = output_file_map + header->e_phoff;
	make_injection(header, section_headers, segment_headers, output_file_map);
	munmap(output_file_map, output_file_size);
	close(output_fd);
}