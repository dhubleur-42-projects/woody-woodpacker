/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_injection_elf64.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 14:58:19 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/16 15:33:49 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prepare_injection.h"

char payload[] = "\x50\x57\x56\x52\x53\x31\xc0\x99\xb2\x0a\xff\xc0\x89\xc7\x48\x8d\x35\x4a\x00\x00\x00\x0f\x05\x48\x8d\x7e\xa1\x48\x2b\x7e\x1b\x48\x8b\x35\x5c\x00\x00\x00\x48\x8d\x15\x3c\x00\x00\x00\x52\xeb\x00\x48\x83\xfe\x00\x74\x1e\x8a\x07\x8a\x1a\x30\xd8\x88\x07\x48\xff\xc7\x48\xff\xc2\x48\xff\xce\x80\x3a\x00\x74\x02\xeb\xe2\x48\x8b\x14\x24\xeb\xdc\x5a\x5b\x5a\x5e\x5f\x58";
char jmp[] = "\xe9\x00\x00\x00\x00";
char data[] = "\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x0a\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
#define PAYLOAD_LENGTH (sizeof(payload)-1 + sizeof(jmp)-1 + sizeof(data)-1)

char *get_section_name(Elf64_Ehdr *header, Elf64_Shdr *section_headers, int index)
{
	Elf64_Shdr *section_header = &section_headers[index];
	Elf64_Shdr *section_names = &section_headers[header->e_shstrndx];
	return ((char *)header + section_names->sh_offset + section_header->sh_name);
}

Elf64_Shdr *get_section(char *name, Elf64_Ehdr *header, Elf64_Shdr *section_headers)
{
	int i;

	i = 0;
	while (i < header->e_shnum)
	{
		if (strcmp(name, get_section_name(header, section_headers, i)) == 0)
			return (&section_headers[i]);
		i++;
	}
	return (NULL);
}

bool	prepare_injection_elf64(t_file file, t_injection *injection, t_options options)
{
	t_file_elf64 file_efl64 = *((t_file_elf64 *)file.specific_file);
	Elf64_Phdr *code_cave = find_code_cave_elf64(file_efl64, PAYLOAD_LENGTH);
	size_t expand_size = 0;
	injection->old_entrypoint_offset = file_efl64.header->e_entry;
	if (code_cave != NULL)
	{
		if (options.verbose)
			printf("Find a code cave in a segment: start: 0x%.8lx, end: 0x%.8lx\n", code_cave->p_offset, code_cave->p_offset + code_cave->p_memsz);
		injection->new_entrypoint_offset = use_code_cave_elf64(file_efl64.header, code_cave, PAYLOAD_LENGTH);
		if (options.verbose)
			printf("Code cave header modified, new end: 0x%.8lx\n", code_cave->p_offset + code_cave->p_memsz);
	}
	else
	{
		if (options.verbose)
			printf("No code cave found, need to extend file\n");
		expand_size = get_extend_size_elf64(PAYLOAD_LENGTH);
	}
	injection->fd = open("woody", O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (injection->fd == -1)
	{
		perror("Cannot open file");
		return false;
	}
	injection->file_size = file.size + expand_size;
	if (lseek(injection->fd, injection->file_size-1, SEEK_SET) == -1)
	{
		perror("Cannot seek file");
		close(injection->fd);
		return false;
	}
	write(injection->fd, "", 1);
	injection->file_map = mmap(NULL, injection->file_size,  PROT_WRITE, MAP_SHARED, injection->fd, 0);
	if (injection->file_map == MAP_FAILED) {
		perror("Cannot map output file");
		close(injection->fd);
		return false;
	}
	if (options.verbose)
		printf("Mapped a file of %lu bytes (extanded by %lu bytes)\n", injection->file_size, expand_size);
	memcpy(injection->file_map, file.map, file.size);
	t_file_elf64 output_file;
	output_file.header = (Elf64_Ehdr *)injection->file_map;
	output_file.sections = (Elf64_Shdr *)(injection->file_map + output_file.header->e_shoff);
	output_file.programs = (Elf64_Phdr *)(injection->file_map + output_file.header->e_phoff);
	if (code_cave == NULL)
		injection->new_entrypoint_offset = extend_and_shift_elf64(PAYLOAD_LENGTH, output_file, injection->file_map, file.size);
	
	Elf64_Shdr *text_section = get_section(".text", output_file.header, output_file.sections);
	if (text_section == NULL)
	{
		dprintf(2, "Cannot find .text section\n");
		return false;
	}
	injection->encrypt_offset = text_section->sh_offset;
	injection->encrypt_size = text_section->sh_size;
	return true;
}