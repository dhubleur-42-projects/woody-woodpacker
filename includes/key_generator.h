/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_generator.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 15:39:07 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/23 16:31:07 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEY_GENERATOR_H
# define KEY_GENERATOR_H

# include "random.h"
# include "parser.h"
# include "libft.h"

void get_key(t_options options, char buffer[17]);

#endif