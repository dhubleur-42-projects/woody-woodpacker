/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:40:25 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/15 13:46:06 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "encrypt.h"

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

void encrypt(Elf64_Ehdr *header, Elf64_Phdr *program_headers, Elf64_Shdr *section_headers, void *file)
{
	(void)program_headers;
	Elf64_Shdr *text_section = get_section(".text", header, section_headers);
	if (text_section == NULL)
		return ;
	char *text = file + text_section->sh_offset;
	int text_size = text_section->sh_size;
	//TODO: to generate randomly
	char key[] = "XXXXXXXXXXXXXXXX";
	xor_cypher(text, text_size, key);
}