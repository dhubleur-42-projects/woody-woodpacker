/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 13:56:27 by dhubleur          #+#    #+#             */
/*   Updated: 2023/05/10 14:15:01 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf_parser.h"

bool parse_header(char *name, Elf64_Ehdr *header_ptr);

void parse_file(char *name) {
	Elf64_Ehdr header;
	if (!parse_header(name, &header))
		return ;
}

bool parse_header(char *name, Elf64_Ehdr *header_ptr) {
	int fd = open(name, O_RDONLY);
	if (fd == -1)
	{
		perror("Cannot open file");
		return false;
	}

	Elf64_Ehdr header = *header_ptr;
	int ret = read(fd, &header, sizeof(Elf64_Ehdr));
	if (ret == -1)
	{
		perror("Cannot read file");
		return false;
	}

	if (ret != sizeof(Elf64_Ehdr))
	{
		dprintf(2, "Invalid file (Impossible to read complete header)\n");
		return false;
	}
	if (header.e_ident[EI_MAG0] != ELFMAG0 || header.e_ident[EI_MAG1] != ELFMAG1 || header.e_ident[EI_MAG2] != ELFMAG2 || header.e_ident[EI_MAG3] != ELFMAG3)
	{
		dprintf(2, "Invalid file (Invalid magic numbers)\n");
		return false;
	}

	if (header.e_ident[EI_CLASS] != ELFCLASS64)
	{
		dprintf(2, "Not a 64 bits file\n");
		return false;
	}

	printf("Valid header file\n");
	return true;
}