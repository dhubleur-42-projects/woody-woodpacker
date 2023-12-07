/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compression.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 21:54:37 by jmaia             #+#    #+#             */
/*   Updated: 2023/12/07 23:26:33 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPRESSION_H
# define COMPRESSION_H

# include <stdint.h>

# define TOKEN_MARKER 137
# define LEN_TOKEN 5
# define SEARCH_BUFFER_LENGTH 2048
# define DATA_BUFFER_LENGTH (SEARCH_BUFFER_LENGTH * 2)

typedef struct s_tracked_data
{
	uint8_t *data;
	uint64_t size;
	uint64_t i;
} t_tracked_data;

typedef struct s_buffered_data
{
	t_tracked_data buffer;
	t_tracked_data data;
} t_buffered_data;

uint64_t	compress(void *data, uint64_t size);
void		decompress(void *data, uint64_t size);

#endif
