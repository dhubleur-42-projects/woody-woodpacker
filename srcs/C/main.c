/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:02:19 by dhubleur          #+#    #+#             */
/*   Updated: 2023/05/16 15:46:45 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include "elf_parser.h"
#include "get_key.h"
#include "libft.h"

void	cipher(uint8_t *message, size_t len, uint8_t *key, uint8_t *iv, uint8_t *encrypted);

//int	main(int ac, char **av) {
//	uint8_t	key[32];
//
//	if (ac < 2 || (ac >= 3 && (ft_strlen(av[2]) != 66 || !is_valid_hex(av[2]))))
//	{
//		dprintf(2, "Usage: %s <filename> [encryption_key (32 bytes long key: 0x0123456789ABCDEF..)]\n", av[0]);
//		return (1);
//	}
//	get_key(key, ac, av);
//	parse_file(av[1]);
//	return (0);
//}

int	main(void)
{
	int fd_read = open("pouet", O_RDONLY);
	int fd_write = open("pouet.enc", O_WRONLY | O_CREAT | O_TRUNC, 0644);

	uint8_t message[256];
	uint8_t encrypted[272];
	uint8_t key[32];
	uint8_t iv[32];
	get_key(key, 0, 0);
	get_key(iv, 0, 0);
	for (int i = 0; i < 4; i++)
	{
		read(fd_read, message, 256);
		cipher(message, 256, key, iv, encrypted);
		write(fd_write, encrypted, 256);
		memcpy(iv, encrypted + 256, 16);
	}
	write(ft_write, encrypted + 256, 16);
}
