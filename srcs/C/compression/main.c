#include <unistd.h>
#include <stdio.h>

#include "compression.h"

int	main()
{
	uint8_t data[] =
		"I AM SAM. I AM SAM. SAM I AM."
		"THAT SAM-I-AM! THAT SAM-I-AM! I DO NOT LIKE THAT SAM-I-AM!"
		"DO WOULD YOU LIKE GREEN EGGS AND HAM?"
		"I DO NOT LIKE THEM,SAM-I-AM."
		"I DO NOT LIKE GREEN EGGS AND HAM.";

	uint64_t new_size = compress(data, sizeof(data));

	for (int i = 0; i < new_size; i++)
	{
		if (data[i] != 137)
		{
			write(1, data[i], 1);
		}
		else
		{
			printf("<%d, %d>", data[i + 1], data[i + 2]);
			i += 2;
		}
	}
}
