/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writter.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 14:02:29 by dhubleur          #+#    #+#             */
/*   Updated: 2023/05/11 14:27:32 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRITTER_H
# define WRITTER_H

# include <unistd.h>	
# include <fcntl.h>
# include <stdlib.h>
# include <elf.h>
# include <stdio.h>
# include <sys/mman.h>
# include <string.h>

void write_new_executable(Elf64_Ehdr header, Elf64_Phdr *program_header, Elf64_Shdr *sections_header, void *input_file_map, off_t input_file_size);

#endif