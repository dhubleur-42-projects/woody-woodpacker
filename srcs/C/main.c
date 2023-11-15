/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:02:19 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/15 18:01:08 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf_parser.h"
#include "libft.h"
#include "parser.h"

void print_help() {
	printf("USAGE: ./woody_woodpacker <options> [file]\n");
	printf("Pack an ELF binary into a self-decrypting binary called 'woody'\n\n");
	printf("The FILE argument must be an ELF 64 binary\n\n");
	printf("OPTIONS:\n");
	printf("\t-h\t\tPrint this help\n");
	printf("\t-c\t\tCompress the binary with [ALGORITHM]\n");
	printf("\t-s [SEED]\tUse SEED to generate the encryption key (can be any ascii string)\n");
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
		printf("usage: ./woody_woodpacker <options> [file]\n");
		return (1);
	}
	start_injection(parser.input);
	return (0);
}
