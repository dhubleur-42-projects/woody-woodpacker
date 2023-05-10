/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 13:56:27 by dhubleur          #+#    #+#             */
/*   Updated: 2023/05/10 14:36:46 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf_parser.h"

bool parse_header(char *name, Elf64_Ehdr *header_ptr, int *fd);
bool parse_sections_header(int fd, Elf64_Ehdr header, Elf64_Shdr **sections);

void parse_file(char *name) {
	int fd;
	Elf64_Ehdr header;
	if (!parse_header(name, &header, &fd))
		return ;

	Elf64_Shdr *sections_header;
	if (!parse_sections_header(fd, header, &sections_header))
	{
		close(fd);
		return ;
	}
}

bool parse_header(char *name, Elf64_Ehdr *header_ptr, int *fd) {
	*fd = open(name, O_RDONLY);
	if (*fd == -1)
	{
		perror("Cannot open file");
		return false;
	}

	Elf64_Ehdr header = *header_ptr;
	int ret = read(*fd, &header, sizeof(Elf64_Ehdr));
	if (ret == -1)
	{
		perror("Cannot read file");
		close (*fd);
		return false;
	}

	if (ret != sizeof(Elf64_Ehdr))
	{
		dprintf(2, "Invalid file (Impossible to read complete header)\n");
		close (*fd);
		return false;
	}
	if (header.e_ident[EI_MAG0] != ELFMAG0 || header.e_ident[EI_MAG1] != ELFMAG1 || header.e_ident[EI_MAG2] != ELFMAG2 || header.e_ident[EI_MAG3] != ELFMAG3)
	{
		dprintf(2, "Invalid file (Invalid magic numbers)\n");
		close (*fd);
		return false;
	}

	if (header.e_ident[EI_CLASS] != ELFCLASS64)
	{
		dprintf(2, "Not a 64 bits file\n");
		close (*fd);
		return false;
	}

	printf("Valid header file\n");
	return true;
}

bool parse_sections_header(int fd, Elf64_Ehdr header, Elf64_Shdr **sections_ptr) {
	if (header.e_shnum == 0 || header.e_shentsize == 0 || header.e_shoff == 0)
	{
		*sections_ptr = NULL;
		printf("No sections header\n");
		return true;
	}

	*sections_ptr = malloc(header.e_shentsize * header.e_shnum);
	if (*sections_ptr == NULL)
	{
		perror("Malloc failed");
		return false;
	}

	Elf64_Shdr *sections = *sections_ptr;
	int ret = lseek(fd, header.e_shoff, SEEK_SET);
	if (ret == -1)
	{
		perror("Cannot seek to sections header");
		free(sections);
		return false;
	}

	ret = read(fd, sections, header.e_shentsize * header.e_shnum);
	if (ret == -1)
	{
		perror("Cannot read sections header");
		free(sections);
		return false;
	}
	if (ret != header.e_shentsize * header.e_shnum)
	{
		dprintf(2, "Invalid file (Impossible to read complete sections header)\n");
		free(sections);
		return false;
	}


	printf("Valid sections header\n");
	return true;
}