/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypt.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:40:11 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/09 13:49:58 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <elf.h>
# include <string.h>

void xor_cypher(char *data, int size, char *key);
void encrypt(Elf64_Ehdr *header, Elf64_Phdr *program_headers, Elf64_Shdr *section_headers, void *file);