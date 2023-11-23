/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   injection.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:54:19 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/23 16:27:59 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INJECTION_H
# define INJECTION_H

# include "prepare_injection.h"
# include "parser.h"
# include "libft.h"

void inject(t_injection injection, t_options options, char key[17]);

#endif