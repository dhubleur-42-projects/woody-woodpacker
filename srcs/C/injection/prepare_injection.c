/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_injection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 14:52:59 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/27 14:00:13 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prepare_injection.h"

bool	prepare_injection(t_file file, t_injection *injection, t_options options)
{
	*injection = (t_injection) {-1, NULL, 0, 0, -1, 0, 0, 0, -1};
	if (file.type == ELF64)
		return (prepare_injection_elf64(file, injection, options));
	if (file.type == ELF32)
		return (prepare_injection_elf32(file, injection, options));
	return (false);
}

void end_injection(t_injection injection)
{
	if (injection.file_map != NULL)
		munmap(injection.file_map, injection.file_size);
	if (injection.fd != -1)
		close(injection.fd);
}