/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:02:19 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/15 17:14:54 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elf_parser.h"
#include "libft.h"

int	main(int ac, char **av) {
	(void)ac;

	if (ac < 2 || (ac >= 3 && (ft_strlen(av[2]) != 66 || !is_valid_hex(av[2]))))
	{
		dprintf(2, "Usage: %s <filename> [encryption_key (32 bytes long key: 0x0123456789ABCDEF..)]\n", av[0]);
		return (1);
	}
	start_injection(av[1]);
	return (0);
}
