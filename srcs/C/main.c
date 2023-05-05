/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 15:02:19 by dhubleur          #+#    #+#             */
/*   Updated: 2023/05/05 15:51:18 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define HEADER_SIZE 5

void print_bytes(char *str) {
	for (int i = 0; i < HEADER_SIZE; i++)
	{
		printf("0x%02x ", str[i]);
	}
	printf("\n");
}

int	main(int ac, char **av) {
	if (ac != 2)
	{
		dprintf(2, "Please enter a file name\n");
		return (1);
	}
	int fd = open(av[1], O_RDONLY);
	if (fd == -1)
	{
		perror("Cannot open file");
		return (1);
	}
	char header[HEADER_SIZE + 1];
	int ret = read(fd, header, HEADER_SIZE);
	if (ret == -1)
	{
		perror("Cannot read file");
		return (1);
	}
	header[HEADER_SIZE] = '\0';
	printf("Header: ");
	print_bytes(header);
	if (ret != HEADER_SIZE)
	{
		dprintf(2, "Invalid file\n");
		return (1);
	}
	if (header[0] != 0x7f || header[1] != 'E' || header[2] != 'L' || header[3] != 'F')
	{
		dprintf(2, "Not a valid ELF file\n");
		return (1);
	}
	if (header[4] != 0x02)
	{
		dprintf(2, "Not a 64 bits ELF file\n");
		return (1);
	}
	printf("Pass all tests\n");
	return (0);
}