/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   injection.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:54:19 by dhubleur          #+#    #+#             */
/*   Updated: 2023/05/16 16:29:24 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INJECTION_H
# define INJECTION_H

# include <elf.h>
# include <sys/mman.h>
# include <fcntl.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>

void create_codecave(Elf64_Ehdr *header, Elf64_Shdr *sections, Elf64_Phdr *program_header, void *output_file_map, off_t output_file_size, char payload[], off_t payload_len);

#endif