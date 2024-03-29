/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_injection_elf32.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 14:58:19 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/24 16:59:42 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prepare_injection.h"

static char *get_section_name(Elf32_Ehdr *header, Elf32_Shdr *section_headers, int index)
{
	Elf32_Shdr *section_header = &section_headers[index];
	Elf32_Shdr *section_names = &section_headers[header->e_shstrndx];
	return ((char *)header + section_names->sh_offset + section_header->sh_name);
}

static Elf32_Shdr *get_section(char *name, Elf32_Ehdr *header, Elf32_Shdr *section_headers)
{
	int i;

	i = 0;
	while (i < header->e_shnum - 1)
	{
		if (ft_strcmp(name, get_section_name(header, section_headers, i)) == 0)
			return (&section_headers[i]);
		i++;
	}
	return (NULL);
}

bool	prepare_injection_elf32(t_file file, t_injection *injection, t_options options)
{
	t_file_elf32 file_efl32 = *((t_file_elf32 *)file.specific_file);
	Elf32_Phdr *code_cave = find_code_cave_elf32(file_efl32, get_payload_length());
	injection->old_entrypoint = file_efl32.header->e_entry;
	if (code_cave != NULL)
	{
		if (options.verbose)
			printf("Find a code cave in a segment: start: 0x%.8x, end: 0x%.8x\n", code_cave->p_offset, code_cave->p_offset + code_cave->p_memsz);
	}
	else
	{
		if (options.verbose)
			printf("No code cave found, impossible to inject payload\n");
		return false;
	}
	injection->fd = open("woody", O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (injection->fd == -1)
	{
		perror("Cannot open file");
		return false;
	}
	injection->file_size = file.size;
	if (lseek(injection->fd, injection->file_size-1, SEEK_SET) == -1)
	{
		perror("Cannot seek file");
		return false;
	}
	write(injection->fd, "", 1);
	injection->file_map = mmap(NULL, injection->file_size,  PROT_WRITE, MAP_SHARED, injection->fd, 0);
	if (injection->file_map == MAP_FAILED) {
		perror("Cannot map output file");
		return false;
	}
	ft_memcpy(injection->file_map, file.map, file.size);
	t_file_elf32 output_file;
	output_file.header = (Elf32_Ehdr *)injection->file_map;
	output_file.sections = (Elf32_Shdr *)(injection->file_map + output_file.header->e_shoff);
	output_file.programs = (Elf32_Phdr *)(injection->file_map + output_file.header->e_phoff);

	code_cave = find_code_cave_elf32(output_file, get_payload_length());
	injection->payload_offset = use_code_cave_elf32(output_file.header, code_cave, get_payload_length(), injection);
	if (options.verbose)
		printf("Code cave header modified, new end: 0x%.8x\n", code_cave->p_offset + code_cave->p_memsz);

	Elf32_Shdr *text_section = get_section(".text", output_file.header, output_file.sections);
	if (text_section == NULL)
	{
		dprintf(2, "Cannot find .text section\n");
		return false;
	}
	text_section->sh_flags |= SHF_WRITE;
	injection->encrypt_offset = text_section->sh_offset;
	injection->encrypt_size = text_section->sh_size;

	Elf32_Phdr *text_segment = NULL;
	for (size_t i = 0; i < output_file.header->e_phnum; i++)
	{
		if (output_file.programs[i].p_offset <= text_section->sh_offset && output_file.programs[i].p_offset + output_file.programs[i].p_filesz >= text_section->sh_offset + text_section->sh_size)
		{
			text_segment = &(output_file.programs[i]);
			break ;
		}
	}
	if (text_segment != NULL) {
		text_segment->p_flags |= PF_W | PF_R;
	}

	return true;
}