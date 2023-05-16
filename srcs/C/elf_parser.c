/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 13:56:27 by dhubleur          #+#    #+#             */
/*   Updated: 2023/05/11 14:36:58 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf_parser.h"
#include "writter.h"

bool parse_header(char *name, Elf64_Ehdr *header_ptr, int *fd);
bool parse_sections_header(int fd, Elf64_Ehdr header, Elf64_Shdr **sections);
bool parse_programs_header(int fd, Elf64_Ehdr header, Elf64_Phdr **program);

void parse_file(char *name) {
	int fd;
	Elf64_Ehdr header;
	if (!parse_header(name, &header, &fd))
		return ;

	Elf64_Phdr *program_header;
	if (!parse_programs_header(fd, header, &program_header))
	{
		close(fd);
		return ;
	}

	Elf64_Shdr *sections_header;
	if (!parse_sections_header(fd, header, &sections_header))
	{
		free(program_header);
		close(fd);
		return ;
	}

	lseek(fd, 0, SEEK_SET);
	off_t input_file_size = lseek(fd, 0, SEEK_END);
    if (input_file_size == -1) {
        perror("Cannot seek to end of file");
		close(fd);
		free(program_header);
		free(sections_header);
		return ;
    }

    void *input_file_map = mmap(NULL, input_file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (input_file_map == MAP_FAILED) {
		perror("Cannot map file");
		close(fd);
		free(program_header);
		free(sections_header);
		return ;
    }
	write_new_executable(header, program_header, sections_header, input_file_map, input_file_size);
	munmap(input_file_map, input_file_size);
	close(fd);
	free(program_header);
	free(sections_header);
}

bool parse_header(char *name, Elf64_Ehdr *header, int *fd) {
	*fd = open(name, O_RDONLY);
	if (*fd == -1)
	{
		perror("Cannot open file");
		return false;
	}

	int ret = read(*fd, header, sizeof(Elf64_Ehdr));
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
	if (header->e_ident[EI_MAG0] != ELFMAG0 || header->e_ident[EI_MAG1] != ELFMAG1 || header->e_ident[EI_MAG2] != ELFMAG2 || header->e_ident[EI_MAG3] != ELFMAG3)
	{
		dprintf(2, "Invalid file (Invalid magic numbers)\n");
		close (*fd);
		return false;
	}

	if (header->e_ident[EI_CLASS] != ELFCLASS64)
	{
		dprintf(2, "Not a 64 bits file\n");
		close (*fd);
		return false;
	}

	printf("Valid header file\n");
	return true;
}

bool parse_programs_header(int fd, Elf64_Ehdr header, Elf64_Phdr **program) {
	if (header.e_phnum == 0 || header.e_phentsize == 0 || header.e_phoff == 0)
	{
		*program = NULL;
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
		perror("Cannot seek to program header");
		free(*program);
		return false;
	}

	ret = read(fd, *program, header.e_phentsize * header.e_phnum);
	if (ret == -1)
	{
		perror("Cannot read program header");
		free(*program);
		return false;
	}
	if (ret != header.e_phentsize * header.e_phnum)
	{
		dprintf(2, "Invalid file (Impossible to read complete program header)\n");
		free(*program);
		return false;
	}

	printf("Valid programs header\n");
	return true;
}

bool parse_sections_header(int fd, Elf64_Ehdr header, Elf64_Shdr **sections) {
	if (header.e_shnum == 0 || header.e_shentsize == 0 || header.e_shoff == 0)
	{
		*sections = NULL;
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
		perror("Cannot seek to sections header");
		free(*sections);
		return false;
	}

	ret = read(fd, *sections, header.e_shentsize * header.e_shnum);
	if (ret == -1)
	{
		perror("Cannot read sections header");
		free(*sections);
		return false;
	}
	if (ret != header.e_shentsize * header.e_shnum)
	{
		dprintf(2, "Invalid file (Impossible to read complete sections header)\n");
		free(*sections);
		return false;
	}


	printf("Valid sections header\n");
	return true;
}