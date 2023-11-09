/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inject.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 13:07:13 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/09 18:06:02 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "injection.h"
#include "encrypt.h"

char payload_part1[] = "\x50\x57\x56\x52\x50\x53\x31\xc0\x99\xb2\x0a\xff\xc0\x89\xc7\x48\x8d\x35\x4a\x00\x00\x00\x0f\x05";
char cipher_params[] = "\xbf\x00\x00\x00\x00\xbe\x00\x00\x00\x00";
char payload_part2[] = "\x48\x8b\x15\x41\x00\x00\x00\x52\xeb\x00\x48\x83\xfe\x00\x74\x22\x48\x8b\x07\x48\x8b\x1a\x48\x31\xd8\x48\x89\x07\x48\xff\xc7\x48\xff\xc2\x48\xff\xce\x80\x3a\x00\x74\x02\xeb\xde\x48\x8b\x14\x24\xeb\xd8\x5a\x5b\x58\x5a\x5e\x5f\x58";
char jmp[] = "\xe9\x00\x00\x00\x00";

char text[] = "\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x0a";
char key[] = "\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x00";

#define PAYLOAD_LENGTH (sizeof(payload_part1)-1 + sizeof(cipher_params)-1 + sizeof(payload_part2)-1 + sizeof(jmp)-1 + sizeof(text)-1 + sizeof(key)-1)
#define PAYLOAD_CODE_LENGTH (sizeof(payload_part1)-1 + sizeof(cipher_params)-1 + sizeof(payload_part2)-1 + sizeof(jmp)-1)

void insert_payload(unsigned char *ptr, unsigned int last_entry, unsigned int current_entry, Elf64_Ehdr *header, Elf64_Shdr *section_headers, Elf64_Phdr *program_headers)
{	
	(void)program_headers;
	printf("Old entry point: 0x%.8x\n", last_entry);
	int32_t jmp_adr = (int32_t)(last_entry - (current_entry + PAYLOAD_CODE_LENGTH));
	printf("Computed jump: %d\n", jmp_adr);
	memcpy(jmp + 1, &jmp_adr, sizeof(jmp_adr));

	//TODO: to generate randomly
	memcpy(key, "XXXXXXXXXXXXXXXX", 16);

	Elf64_Shdr *text_section = get_section(".text", header, section_headers);
	if (text_section == NULL)
		return ;
	int32_t text_adr = (int32_t)(text_section->sh_offset);  //TODO: need to add this offset to something
	int32_t test_len = (int32_t)(text_section->sh_size);

	memcpy(cipher_params + 1, &text_adr, sizeof(text_adr));
	memcpy(cipher_params + 6, &test_len, sizeof(test_len));

    memcpy(ptr, payload_part1, sizeof(payload_part1)-1);
	ptr += sizeof(payload_part1)-1;
	memcpy(ptr, cipher_params, sizeof(cipher_params)-1);
	ptr += sizeof(cipher_params)-1;
	memcpy(ptr, payload_part2, sizeof(payload_part2)-1);
	ptr += sizeof(payload_part2)-1;
	memcpy(ptr, jmp, sizeof(jmp)-1);
	ptr += sizeof(jmp)-1;
	memcpy(ptr, text, sizeof(text)-1);
	ptr += sizeof(text)-1;
	memcpy(ptr, key, sizeof(key)-1);
	ptr += sizeof(key)-1;

}

void make_injection(Elf64_Ehdr *header, Elf64_Shdr *section_headers, Elf64_Phdr *segment_headers, void *file_map, size_t output_file_size, off_t old_file_size) {
	Elf64_Phdr *code_cave = find_code_cave(header, segment_headers, PAYLOAD_LENGTH);
	if (code_cave) {
		printf("Code cave segment: start: 0x%.8lx, end: 0x%.8lx\n", code_cave->p_offset, code_cave->p_offset + code_cave->p_memsz);
		unsigned int last_entry = header->e_entry;
		size_t injection_offset = use_code_cave(header, code_cave, PAYLOAD_LENGTH);
		printf("Code cave header modified, new end: 0x%.8lx\n", code_cave->p_offset + code_cave->p_memsz);
		printf("New entry point: 0x%.8lx\n", header->e_entry);
		insert_payload(file_map + injection_offset, last_entry, header->e_entry, header, section_headers, segment_headers);
		printf("Payload injected\n");
	} else {
		unsigned int last_entry = header->e_entry;
		size_t insert = extend_and_shift(PAYLOAD_LENGTH, header, segment_headers, section_headers, file_map, output_file_size, old_file_size);
		printf("New entry point: 0x%.8lx\n", header->e_entry);
		insert_payload(file_map + insert, last_entry, header->e_entry, header, section_headers, segment_headers);
		printf("Payload injected\n");
	}
}

void inject(Elf64_Ehdr *input_header, Elf64_Phdr *input_segment_headers, Elf64_Shdr *input_section_headers, void *input_file_map, off_t input_file_size) {
	off_t output_file_size = input_file_size;
	off_t old_file_size = input_file_size;
	if (find_code_cave(input_header, input_segment_headers, PAYLOAD_LENGTH) == NULL)
	{
		printf("===== ELF SHIFTING =====\n");
		size_t needed_size;
		if (!get_extend_size(PAYLOAD_LENGTH, input_header, input_section_headers, &needed_size))
			return ;
		output_file_size += needed_size;
	} else {
		printf("===== CODE CAVE =====\n");
	}
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
	make_injection(header, section_headers, segment_headers, output_file_map, output_file_size, old_file_size);

	printf("===== ENCRYPTING =====\n");
	encrypt(header, segment_headers, section_headers, output_file_map);
	printf("===== DONE =====\n");

	munmap(output_file_map, output_file_size);
	close(output_fd);
}