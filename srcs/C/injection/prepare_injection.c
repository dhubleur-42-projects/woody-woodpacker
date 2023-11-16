/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_injection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 14:52:59 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/16 15:54:50 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prepare_injection.h"

bool	prepare_injection(t_file file, t_injection *injection, t_options options)
{
	*injection = (t_injection) {-1, NULL, 0, 0, -1, 0, 0, 0};
	if (file.type == ELF64)
		return (prepare_injection_elf64(file, injection, options));
	return (false);
}