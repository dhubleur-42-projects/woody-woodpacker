/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:09:38 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/15 17:09:22 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

#include "encryption_utils.h"

int	main(void)
{
//	uint32_t initial_key[] = {0x61c38762, 0x6b0659d1, 0x419b2b98, 0xee9f7034, 0x6a4da5c0, 0xe77f1912, 0x5acf9bd0, 0x239e44c1};
	uint8_t initial_key[] = {0x62, 0x87, 0xc3, 0x61, 0xd1, 0x59, 0x06, 0x6b, 0x98, 0x2b, 0x9b, 0x41, 0x34, 0x70, 0x9f, 0xee, 0xc0, 0xa5, 0x4d, 0x6a, 0x12, 0x19, 0x7f, 0xe7, 0xd0, 0x9b, 0xcf, 0x5a, 0xc1, 0x44, 0x9e, 0x23};
	uint8_t iv[] = {0xb2, 0x78, 0xf8, 0x8d, 0x19, 0xc0, 0x13, 0x39, 0x2e, 0x56, 0x23, 0x79, 0x41, 0x93, 0x18, 0x87};
	uint8_t message[] = "Le chat est mort :)";
	uint8_t encrypted[32];
	uint8_t decrypted[32];

	cipher(message, 19, initial_key, iv, encrypted);
	printf("Ciphered text:\n");
	for (int i = 0; i < 16; i++)
		printf("%02x ", encrypted[i]);
	printf("\n");
	size_t len = uncipher(encrypted, 32, initial_key, iv, decrypted);
	printf("Len: %ld\n", len);
	write(1, decrypted, len);
}
