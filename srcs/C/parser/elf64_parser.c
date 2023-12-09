/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 13:56:27 by dhubleur          #+#    #+#             */
/*   Updated: 2023/12/09 10:47:40 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf_parser.h"

static bool parse_header(int fd, Elf64_Ehdr **header, t_options options) {
	*header = malloc(sizeof(Elf64_Ehdr));
	if (*header == NULL)
	{
		perror("Malloc failed");
		return false;
	}
	int ret = read(fd, *header, sizeof(Elf64_Ehdr));
	if (ret == -1)
	{
		free(*header);
		perror("Cannot read file");
		return false;
	}

	if (ret != sizeof(Elf64_Ehdr))
	{
		free(*header);
		dprintf(2, "Invalid file (Impossible to read complete header)\n");
		return false;
	}
	
	if (options.verbose)
		printf("Parsed a valid ELF64 header\n");
	return true;
}

static bool parse_programs_header(int fd, Elf64_Ehdr header, Elf64_Phdr **program, t_options options) {
	if (header.e_phnum == 0 || header.e_phentsize == 0 || header.e_phoff == 0)
	{
		printf("No program header\n");
		return true;
	}

	*program = malloc(header.e_phentsize * header.e_phnum);
	if (*program == NULL)
	{
		perror("Malloc failed");
		return false;
	}

	int ret = lseek(fd, header.e_phoff, SEEK_SET);
	if (ret == -1)
	{
		free(*program);
		perror("Cannot seek to program header");
		return false;
	}

	ret = read(fd, *program, header.e_phentsize * header.e_phnum);
	if (ret == -1)
	{
		free(*program);
		perror("Cannot read program header");
		return false;
	}
	if (ret != header.e_phentsize * header.e_phnum)
	{
		free(*program);
		dprintf(2, "Invalid file (Impossible to read complete program header)\n");
		return false;
	}

	if (options.verbose)
		printf("Parsed %i valid program headers\n", header.e_phnum);
	return true;
}

static bool parse_sections_header(int fd, Elf64_Ehdr header, Elf64_Shdr **sections, t_options options) {
	if (header.e_shnum == 0 || header.e_shentsize == 0 || header.e_shoff == 0)
	{
		printf("No sections header\n");
		return true;
	}

	*sections = malloc(header.e_shentsize * header.e_shnum);
	if (*sections == NULL)
	{
		perror("Malloc failed");
		return false;
	}

	int ret = lseek(fd, header.e_shoff, SEEK_SET);
	if (ret == -1)
	{
		free(*sections);
		perror("Cannot seek to sections header");
		return false;
	}

	ret = read(fd, *sections, header.e_shentsize * header.e_shnum);
	if (ret == -1)
	{
		free(*sections);
		perror("Cannot read sections header");
		return false;
	}
	if (ret != header.e_shentsize * header.e_shnum)
	{
		free(*sections);
		dprintf(2, "Invalid file (Impossible to read complete sections header)\n");
		return false;
	}

	if (options.verbose)
		printf("Parsed %i valid sections headers\n", header.e_shnum);
	return true;
}

bool parse_elf64(int fd, t_file *file, t_options options)
{
	t_file_elf64 *file_elf64 = malloc(sizeof(t_file_elf64));
	if (file_elf64 == NULL)
	{
		perror("Malloc failed");
		return false;
	}
	file_elf64->header = NULL;
	file_elf64->programs = NULL;
	file_elf64->sections = NULL;
	file->specific_file = file_elf64;

	lseek(fd, 0, SEEK_SET);
	Elf64_Ehdr *header;
	if (!parse_header(fd, &header, options))
		return false;
	file_elf64->header = header;

	Elf64_Phdr *program_header;
	if (!parse_programs_header(fd, *header, &program_header, options))
	{
		return false;
	}
	file_elf64->programs = program_header;

	Elf64_Shdr *sections_header;
	if (!parse_sections_header(fd, *header, &sections_header, options))
	{
		return false;
	}
	file_elf64->sections = sections_header;

	lseek(fd, 0, SEEK_SET);
	off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
        perror("Cannot seek to end of file");
		return false;
    }
	file->size = file_size;

    void *input_file_map = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (input_file_map == MAP_FAILED) {
		perror("Cannot map file");
		return false;
    }
	file->map = input_file_map;

	return true;
}
