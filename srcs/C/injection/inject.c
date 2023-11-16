/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inject.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 13:07:13 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/16 15:55:56 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "injection.h"

char payload[] = "\x50\x57\x56\x52\x53\x31\xc0\x99\xb2\x0a\xff\xc0\x89\xc7\x48\x8d\x35\x4a\x00\x00\x00\x0f\x05\x48\x8d\x7e\xa1\x48\x2b\x7e\x1b\x48\x8b\x35\x5c\x00\x00\x00\x48\x8d\x15\x3c\x00\x00\x00\x52\xeb\x00\x48\x83\xfe\x00\x74\x1e\x8a\x07\x8a\x1a\x30\xd8\x88\x07\x48\xff\xc7\x48\xff\xc2\x48\xff\xce\x80\x3a\x00\x74\x02\xeb\xe2\x48\x8b\x14\x24\xeb\xdc\x5a\x5b\x5a\x5e\x5f\x58";
char jmp[] = "\xe9\x00\x00\x00\x00";
char data[] = "\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x0a\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x58\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

#define DATA_KEY &data[10]
#define DATA_TEXT_OFFSET &data[27]
#define DATA_DATA_LEN &data[35]

#define PAYLOAD_LENGTH (sizeof(payload)-1 + sizeof(jmp)-1 + sizeof(data)-1)
#define PAYLOAD_CODE_LENGTH (sizeof(payload)-1 + sizeof(jmp)-1)

size_t get_payload_length()
{
	return (PAYLOAD_LENGTH);
}

void inject(t_injection injection, t_options options)
{
	int32_t jmp_adr = (int32_t)(injection.old_entrypoint - (injection.new_entrypoint + PAYLOAD_CODE_LENGTH));
	if (options.verbose)
	{
		printf("Old entry point: 0x%.8lx, New entry point: 0x%.8lx\n", injection.old_entrypoint, injection.new_entrypoint);
		printf("Computed jump: %d\n", jmp_adr);
	}
	memcpy(jmp + 1, &jmp_adr, sizeof(jmp_adr));

	memcpy(DATA_KEY, "XXXXXXXXXXXXXXXX", 16);
	
	int32_t text_offset = injection.payload_offset - injection.encrypt_offset;
	memcpy(DATA_TEXT_OFFSET, &text_offset, sizeof(text_offset));

	int32_t text_len = (int32_t)(injection.encrypt_size);
	memcpy(DATA_DATA_LEN, &text_len, sizeof(text_len));

	memcpy(injection.file_map + injection.payload_offset, payload, sizeof(payload)-1);
	memcpy(injection.file_map + injection.payload_offset + sizeof(payload)-1, jmp, sizeof(jmp)-1);
	memcpy(injection.file_map + injection.payload_offset + sizeof(payload)-1 + sizeof(jmp)-1, data, sizeof(data)-1);
	printf("Payload injected at: 0x%.8lx\n", injection.payload_offset);
}