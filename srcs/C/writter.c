/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 14:02:24 by dhubleur          #+#    #+#             */
/*   Updated: 2023/05/15 15:41:39 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "writter.h"

char payload[] =
    "\x31\xc0\x99\xb2\x0a\xff\xc0\x89\xc7\x48\x8d\x35\x12\x00\x00\x00\x0f\x05"
    "\xb2\x2a\x31\xc0\xff\xc0\xf6\xe2\x89\xc7\x31\xc0\xb0\x3c\x0f\x05\x2e\x2e"
    "\x57\x4f\x4f\x44\x59\x2e\x2e\x0a";

void write_new_executable(Elf64_Ehdr header, Elf64_Phdr *program_header, Elf64_Shdr *sections_header, void *input_file_map, off_t input_file_size) {
	int output_fd = open("woody", O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (output_fd == -1)
	{
		perror("Cannot open file");
		return ;
	}

	size_t payload_len = strlen(payload);

	if (ftruncate(output_fd, input_file_size + payload_len) == -1) {
		perror("Cannot truncate file");
		close(output_fd);
		return ;
	}
	
	void *output_file_map = mmap(NULL, input_file_size + payload_len, PROT_WRITE, MAP_PRIVATE, output_fd, 0);
	if (output_file_map == MAP_FAILED) {
		perror("Cannot map output file");
		close(output_fd);
		return ;
	}

	(void)header;
	(void)program_header;
	(void)sections_header;

	// Copy the input file to the output file
	memcpy(output_file_map, input_file_map, input_file_size);

	// Elf64_Ehdr *output_header = output_file_map;
	// Elf64_Off entry_offset = output_header->e_entry;
    // void *entry_point = output_file_map + entry_offset;
    // memmove(entry_point + payload_len, entry_point, input_file_size - entry_offset);
    // memcpy(entry_point, payload, payload_len);

	munmap(output_file_map, input_file_size);
	close(output_fd);
}