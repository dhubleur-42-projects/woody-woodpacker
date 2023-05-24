/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 13:35:44 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/23 11:57:01 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>

size_t ft_strlen(char *str);
int	is_valid_hex(char *hex);
void	ft_memcpy(void *dest, void *src, size_t n_bytes);

#endif
