#include "encryption_utils.h"

#include <strings.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int	main(void)
{
	{
		uint8_t message[48] = "Le chat est mor";
		uint8_t key[] = "J'adore les chips en ete avec du";
		uint8_t iv[] = "Idem,c'est top !";
		uint8_t encrypted[48];
		uint8_t decrypted[48];

		bzero(encrypted, 48);
		bzero(decrypted, 48);
		asm_cipher(message, 15, key, iv, encrypted);
		for (int i = 0; i < 48; i++)
			printf("%02x ", encrypted[i]);
		printf("\n");
		asm_uncipher(encrypted, 16, key, iv, decrypted, 1);
		for (int i = 0; i < 48; i++)
			printf("%c", decrypted[i]);
		printf("\n");
	}
	printf("\n");
	{
		uint8_t message[] = "Le chat est mor";
		uint8_t key[] = "J'adore les chips en ete avec du";
		uint8_t iv[] = "Idem,c'est top !";
		uint8_t encrypted[48];
		uint8_t decrypted[48];

		bzero(encrypted, 48);
		bzero(decrypted, 48);
		c_cipher(message, 15, key, iv, encrypted);
		for (int i = 0; i < 48; i++)
			printf("%02x ", encrypted[i]);
		printf("\n");
		c_uncipher(encrypted, 16, key, iv, decrypted, 1);
		for (int i = 0; i < 48; i++)
			printf("%c", decrypted[i]);
		printf("\n");
	}
	{
		printf("--- Big_file encryption --- \n");
		int fd_read = open("big_file", O_RDONLY);
		int fd_write = open("big_file.enc", O_WRONLY | O_TRUNC | O_CREAT, 0644);

		uint8_t block[1024];
		uint8_t key[] = "J'adore les chips en ete avec du";
		uint8_t iv[] = "Idem,c'est top !";
		uint8_t enc_block[1040];
		size_t	read_bytes = 1;
		size_t	total_read = 0;

		// On chiffre uniquement les blocs de 1024 tant qu'on n'a pas fini de lire
		while (read_bytes > 0)
		{
			read_bytes = read(fd_read, block + total_read, 1024 - total_read);
			total_read += read_bytes > 0 ? read_bytes : 0;
			if (total_read == 1024)
			{
				cipher(block, 1024, key, iv, enc_block);
				write(fd_write, enc_block, 1024);
				total_read = 0;
			}
		}
		if (total_read != 0)
		{
			size_t enc_block_size = cipher(block, total_read, key, iv, enc_block);
			write(fd_write, enc_block, enc_block_size);
		}
		else
			write(fd_write, enc_block + 1024, 16);
	}
	{
		printf("--- Big file decryption ---\n");
		int fd_read = open("big_file.enc", O_RDONLY);
		int fd_write = open("big_file.dec", O_WRONLY | O_TRUNC | O_CREAT, 0644);

		uint8_t block[1024];
		uint8_t key[] = "J'adore les chips en ete avec du";
		uint8_t iv[] = "Idem,c'est top !";
		uint8_t dec_block[1024];
		size_t	read_bytes = 1;
		size_t	total_read = 0;

		// On dechiffre uniquement les blocs de 1024 tant qu'on n'a pas fini de lire
		while (read_bytes > 0)
		{
			read_bytes = read(fd_read, block + total_read, 1024 - total_read);
			total_read += read_bytes > 0 ? read_bytes : 0;
			if (total_read == 1024)
			{
				uncipher(block, 1024, key, iv, dec_block, 0);
				write(fd_write, dec_block, 1024);
				total_read = 0;
			}
		}
		// TODO Check if error on read/write/etc
		// TODO File enc of 1024, on va ecrire direct dedans en mode no ncbc et c'est moche
		if (total_read != 0)
		{
			size_t dec_block_size = uncipher(block, total_read, key, iv, dec_block, 1);
			write(fd_write, dec_block, dec_block_size);
		}
		else
		{
			write(fd_write, dec_block + 1024, 16);
		}
	}
}
