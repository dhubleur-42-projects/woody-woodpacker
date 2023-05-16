/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   writter.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 14:02:29 by dhubleur          #+#    #+#             */
/*   Updated: 2023/05/16 15:45:42 by dhubleur         ###   ########.fr       */
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

void write_new_executable(void *input_file_map, off_t input_file_size);

#endif