/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 14:02:24 by dhubleur          #+#    #+#             */
/*   Updated: 2023/05/11 14:37:51 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "writter.h"

void write_new_executable(Elf64_Ehdr header, Elf64_Phdr *program_header, Elf64_Shdr *sections_header, void *input_file_map, off_t input_file_size) {
	int output_fd = open("woody", O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (output_fd == -1)
	{
		perror("Cannot open file");
		return ;
	}

	void *output_file_map = mmap(NULL, input_file_size, PROT_WRITE, MAP_SHARED, output_fd, 0);
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

	
	munmap(output_file_map, input_file_size);
	close(output_fd);
}