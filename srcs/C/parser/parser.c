/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 17:37:30 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/16 16:58:29 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	parse(int argc, char **argv, t_parser *parser)
{
	parser->options = (t_options) {NULL, NULL, false, false, false};
	parser->input = NULL;

	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			if (strlen(argv[i]) != 2) {
				printf(USAGE_ERROR);
				return (false);
			}
			if (!parse_options(argc, argv, &i, &parser->options))
				return (false);
		}
		else
		{
			if (parser->input == NULL) {
				parser->input = argv[i];
			} else {
				printf(USAGE_ERROR);
				return (false);
			}
		}
	}

	return true;
}

void close_file(t_file file)
{
	if (file.specific_file != NULL)
	{
		if (file.type == ELF64)
		{
			t_file_elf64 *elf64 = (t_file_elf64 *)file.specific_file;
			free(elf64->header);
			free(elf64->sections);
			free(elf64->programs);
		}
		free(file.specific_file);
	}
	if (file.map != NULL)
		munmap(file.map, file.size);
	if (file.fd != -1)
		close(file.fd);
}

bool	get_file_type(char *name, t_file *file, t_options options)
{
	*file = (t_file){NONE, -1, NULL, 0, NULL};
	
	file->fd = open(name, O_RDONLY);
	if (file->fd == -1)
	{
		perror("Cannot open file");
		close_file(*file);
		return false;
	}
	char buffer[5];
	int ret = read(file->fd, buffer, 5);
	if (ret == -1)
	{
		perror("Cannot read file");
		close_file(*file);
		return false;
	}
	if (ret != 5)
	{
		dprintf(2, "Invalid file (Impossible to read magic numbers)\n");
		close_file(*file);
		return false;
	}
	if (buffer[0] == 0x7f && buffer[1] == 'E' && buffer[2] == 'L' && buffer[3] == 'F')
	{
		if (buffer[4] == 1)
			file->type = ELF32;
		else if (buffer[4] == 2)
			file->type = ELF64;
		else
		{
			dprintf(2, "Invalid file (Invalid ELF class)\n");
			close_file(*file);
			return false;
		}
	}
	if (file->type == NONE)
	{
		dprintf(2, "Invalid file (Invalid magic numbers)\n");
		close_file(*file);
		return false;
	}
	if (options.verbose) {
		printf("Detected file type: ");
		switch (file->type)
		{
			case ELF32:
				printf("ELF32\n");
				break;
			case ELF64:
				printf("ELF64\n");
				break;
			default:
				printf("UNKNOWN\n");
				break;
		}
	}
	return true;
}

bool get_specific_file(t_file *file, t_options options)
{
	if (file->type == ELF64)
	{
		if (!parse_elf64(file->fd, file, options))
		{
			close_file(*file);
			return false;
		}
		if (options.verbose)
		{
			printf("Successfully parsed ELF64 file\n");
		}
		return true;
	}
	return false;
}