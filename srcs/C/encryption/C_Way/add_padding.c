/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_padding.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 20:09:15 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/16 20:09:28 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include "encryption_utils.h"

void add_padding(uint8_t *block, size_t block_len)
{
	size_t	len_pad = 16 - block_len;

	for (size_t i = block_len; i < 16; i++)
		block[i] = len_pad;
}
