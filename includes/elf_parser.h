/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_parser.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 13:56:36 by dhubleur          #+#    #+#             */
/*   Updated: 2023/05/10 14:13:27 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ELF_H
# define ELF_H

# include <elf.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdbool.h>

void parse_file(char *name);

#endif