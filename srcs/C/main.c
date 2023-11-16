/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:02:19 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/16 17:21:17 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"
#include "prepare_injection.h"
#include "encrypt.h"
#include "injection.h"

void print_help() {
	printf(USAGE_ERROR);
	printf("Pack an ELF binary into a self-decrypting binary called 'woody'\n\n");
	printf("The FILE argument must be an ELF 64 binary\n\n");
	printf("OPTIONS:\n");
	printf("\t-h\t\tPrint this help\n");
	printf("\t-c\t\tCompress the binary with [ALGORITHM]\n");
	printf("\t-s [SEED]\tUse SEED to generate the encryption key (can be any ascii string)\n");
	printf("\t-k [KEY]\tUse KEY as encryption key (need to be a 16 characters long ascii string)\n");
	printf("\t-v\t\tVerbose mode\n");
}

int	main(int ac, char **av) {
	t_parser	parser;
	if (!parse(ac, av, &parser))
		return (1);
	if (parser.options.help) {
		print_help();
		return (0);
	}
	if (parser.input == NULL) {
		printf(USAGE_ERROR);
		return (1);
	}
	t_file file;
	if (parser.options.verbose)
		printf("================================\n");
	if (!get_file_type(parser.input, &file, parser.options))
		return (1);
	if (parser.options.verbose)
		printf("================================\n");
	if (!get_specific_file(&file, parser.options))
		return (1);
	if (parser.options.verbose)
		printf("================================\n");
	t_injection injection;
	if (!prepare_injection(file, &injection, parser.options))
		return (1);
	close_file(file);
	if (parser.options.verbose)
		printf("================================\n");
	xor_cipher(injection.file_map + injection.encrypt_offset, injection.encrypt_size, "XXXXXXXXXXXXXXXX");
	if (parser.options.verbose)
	{
		printf("Cipher applied on %d bytes\n", injection.encrypt_size);
		printf("================================\n");
	}
	inject(injection, parser.options);
	if (parser.options.verbose)
		printf("================================\n");
	end_injection(injection);
	return (0);
}
