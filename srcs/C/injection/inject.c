/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inject.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 13:07:13 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/28 15:18:54 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "injection.h"

char payload_64[] = "\x50\x57\x56\x52\x53\x31\xc0\x99\xb2\x0a\xff\xc0\x89\xc7\x48\x8d\x35\x4a\x00\x00\x00\x0f\x05\x48\x8d\x7e\xa1\x48\x2b\x7e\x1b\x48\x8b\x35\x5c\x00\x00\x00\x48\x8d\x15\x3c\x00\x00\x00\x52\xeb\x00\x48\x83\xfe\x00\x74\x1e\x8a\x07\x8a\x1a\x30\xd8\x88\x07\x48\xff\xc7\x48\xff\xc2\x48\xff\xce\x80\x3a\x00\x74\x02\xeb\xe2\x48\x8b\x14\x24\xeb\xdc\x5a\x5b\x5a\x5e\x5f\x58";
char jmp_64[] = "\xe9\x00\x00\x00\x00";
char data_64[] = "\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x0a\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

char payload_32[] = "\x50\x53\x51\x52\x99\xb2\x0a\xbb\x01\x00\x00\x00\x8d\x0d\x54\x00\x00\x00\xb8\x04\x00\x00\x00\xcd\x80\x8d\x41\xac\x2b\x41\x1b\x8b\x1d\x73\x00\x00\x00\x8d\x0d\x5e\x00\x00\x00\x51\xeb\x00\x83\xfb\x00\x74\x17\x8a\x00\x8a\x19\x30\xd8\x88\x00\x40\x41\x4b\x80\x39\x00\x74\x02\xeb\xe9\x8b\x0c\x24\xeb\xe4\x59\x5a\x59\x5b\x58";
char jmp_32[] = "\xe9\x00\x00\x00\x00";
char data_32[] = "\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x0a\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x00\x00\x00\x00\x00\x00\x00\x00\x00";


#define DATA_KEY_64 &data_64[10]
#define DATA_TEXT_OFFSET_64 &data_64[27]
#define DATA_TEXT_LEN_64 &data_64[35]

#define DATA_KEY_32 &data_32[10]
#define DATA_TEXT_OFFSET_32 &data_32[27]
#define DATA_TEXT_LEN_32 &data_32[31]

#define PAYLOAD_LENGTH_64 (sizeof(payload_64)-1 + sizeof(jmp_64)-1 + sizeof(data_64)-1)
#define PAYLOAD_CODE_LENGTH_64 (sizeof(payload_64)-1 + sizeof(jmp_64)-1)

#define PAYLOAD_LENGTH_32 (sizeof(payload_32)-1 + sizeof(jmp_32)-1 + sizeof(data_32)-1)
#define PAYLOAD_CODE_LENGTH_32 (sizeof(payload_32)-1 + sizeof(jmp_32)-1)

size_t get_payload_length(t_file_type type)
{
	if (type == ELF64)
		return PAYLOAD_LENGTH_64;
	else if (type == ELF32)
		return PAYLOAD_LENGTH_32;
	return 0;
}

static void inject_elf64(t_injection injection, t_options options, char key[17])
{
	int32_t jmp_adr = (int32_t)(injection.old_entrypoint - (injection.new_entrypoint + PAYLOAD_CODE_LENGTH_64));
	if (options.verbose)
	{
		printf("Old entry point: 0x%.8lx, New entry point: 0x%.8lx\n", injection.old_entrypoint, injection.new_entrypoint);
		printf("Computed jump: %d\n", jmp_adr);
	}
	ft_memcpy(jmp_64 + 1, &jmp_adr, sizeof(jmp_adr));

	ft_memcpy(DATA_KEY_64, key, 16);
	
	int32_t text_offset = injection.payload_offset - injection.encrypt_offset;
	ft_memcpy(DATA_TEXT_OFFSET_64, &text_offset, sizeof(text_offset));

	int32_t text_len = (int32_t)(injection.encrypt_size);
	ft_memcpy(DATA_TEXT_LEN_64, &text_len, sizeof(text_len));

	char *ptr = injection.file_map + injection.payload_offset;
	ft_memcpy(ptr, payload_64, sizeof(payload_64) - 1);
	ptr += sizeof(payload_64) - 1;
	ft_memcpy(ptr, jmp_64, sizeof(jmp_64) - 1);
	ptr += sizeof(jmp_64) - 1;
	ft_memcpy(ptr, data_64, sizeof(data_64) - 1);
	if (options.verbose)
		printf("Payload injected at: 0x%.8lx\n", injection.payload_offset);
}

static void inject_elf32(t_injection injection, t_options options, char key[17])
{
	int32_t jmp_adr = (int32_t)(injection.old_entrypoint - (injection.new_entrypoint + PAYLOAD_CODE_LENGTH_32));
	if (options.verbose)
	{
		printf("Old entry point: 0x%.8lx, New entry point: 0x%.8lx\n", injection.old_entrypoint, injection.new_entrypoint);
		printf("Computed jump: %d\n", jmp_adr);
	}
	ft_memcpy(jmp_32 + 1, &jmp_adr, sizeof(jmp_adr));

	ft_memcpy(DATA_KEY_32, key, 16);
	
	int32_t text_offset = injection.payload_offset - injection.encrypt_offset;
	ft_memcpy(DATA_TEXT_OFFSET_32, &text_offset, sizeof(text_offset));

	int32_t text_len = (int32_t)(injection.encrypt_size);
	ft_memcpy(DATA_TEXT_LEN_32, &text_len, sizeof(text_len));

	char *ptr = injection.file_map + injection.payload_offset;
	ft_memcpy(ptr, payload_32, sizeof(payload_32) - 1);
	ptr += sizeof(payload_32) - 1;
	ft_memcpy(ptr, jmp_32, sizeof(jmp_32) - 1);
	ptr += sizeof(jmp_32) - 1;
	ft_memcpy(ptr, data_32, sizeof(data_32) - 1);
	if (options.verbose)
		printf("Payload injected at: 0x%.8lx\n", injection.payload_offset);
}

void inject(t_injection injection, t_options options, char key[17])
{
	if (injection.payload_type == ELF64)
		inject_elf64(injection, options, key);
	else if (injection.payload_type == ELF32)
		inject_elf32(injection, options, key);
}