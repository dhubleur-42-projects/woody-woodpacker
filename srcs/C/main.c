/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:02:19 by dhubleur          #+#    #+#             */
/*   Updated: 2023/05/10 14:08:26 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf_parser.h"

int	main(int ac, char **av) {
	if (ac != 2)
	{
		dprintf(2, "Please enter a file name\n");
		return (1);
	}
	parse_file(av[1]);
	return (0);
}