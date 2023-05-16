/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cpu_support_aes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 18:21:11 by jmaia             #+#    #+#             */
/*   Updated: 2023/05/16 18:24:40 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#define cpuid(func,ax,bx,cx,dx)\
	__asm__ __volatile__ ("cpuid":\
			"=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : "a" (func));

int check_cpu_support_aes()
{
	unsigned int a, b, c, d;

	cpuid(1, a, b, c, d);
	return (c & 0x2000000);
}
