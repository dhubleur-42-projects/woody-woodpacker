/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 14:02:24 by dhubleur          #+#    #+#             */
/*   Updated: 2023/05/15 16:08:53 by dhubleur         ###   ########.fr       */
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

	size_t payload_len = sizeof(payload) - 1;

	if (ftruncate(output_fd, input_file_size + payload_len) == -1) {
		perror("Cannot truncate file");
		close(output_fd);
		return ;
	}
	
	void *output_file_map = mmap(NULL, input_file_size + payload_len, PROT_WRITE, MAP_SHARED, output_fd, 0);
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

    Elf64_Ehdr *output_header = output_file_map;
    Elf64_Phdr *output_program_header = output_file_map + output_header->e_phoff;
    Elf64_Off text_offset = 0;
    size_t text_size = 0;

	// Find .text section (we assume it's the first PT_LOAD segment)
    for (int i = 0; i < output_header->e_phnum; i++) {
        if (output_program_header[i].p_type == PT_LOAD && output_program_header[i].p_flags == (PF_X|PF_R)) {
            text_offset = output_program_header[i].p_offset;
            text_size = output_program_header[i].p_memsz;
            break;
        }
    }

	// Copy the payload to the .text section
    memcpy(output_file_map + text_offset, payload, payload_len);

	munmap(output_file_map, input_file_size);
	close(output_fd);
}