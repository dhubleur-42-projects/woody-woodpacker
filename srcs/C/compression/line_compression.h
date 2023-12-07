/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_compression.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 10:36:24 by jmaia             #+#    #+#             */
/*   Updated: 2023/11/25 10:41:57 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_COMPRESSION_H
# define LINE_COMPRESSION_H

# define LEN_SEARCH_BUFFER 2048
# define LEN_FILE_BUFFER (LEN_SEARCH_BUFFER * 2)

typedef struct s_file_buffer
{
	int fd;
	uint8_t buffer[LEN_FILE_BUFFER];
	int i_buffer;
}	t_file_buffer;

void	init_file_buffer(t_file_buffer *file_buffer);

#endif
