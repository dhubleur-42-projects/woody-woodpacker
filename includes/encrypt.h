/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encrypt.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:40:11 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/16 18:28:32 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENCRYPT_H
# define ENCRYPT_H

# include <elf.h>
# include <string.h>

void xor_cipher(char *data, int size, char *key);

#endif