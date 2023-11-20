/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 13:31:38 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/20 13:55:51 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/time.h>
#include <stdio.h>

unsigned int get_seed_from_timestamp() {
    uint64_t timestamp;
    if (syscall(SYS_clock_gettime, CLOCK_REALTIME, &timestamp) != 0) {
		perror("clock_gettime");
		return 0;
	}
    return (unsigned int) (timestamp ^ (timestamp >> 32));
}

//Mersenne Twister PRNG algorithm (https://en.wikipedia.org/wiki/Mersenne_Twister)
static unsigned int ft_mt[624];
static int ft_index = 0;

void ft_srand(unsigned int seed) {
   ft_mt[0] = seed;
    for (int i = 1; i < 624; i++) {
        ft_mt[i] = 1812433253 * (ft_mt[i - 1] ^ (ft_mt[i - 1] >> 30)) + i;
    }
}

static void generate_numbers() {
    for (int i = 0; i < 624; i++) {
        unsigned int y = (ft_mt[i] & 0x80000000) + (ft_mt[(i + 1) % 624] & 0x7fffffff);
        ft_mt[i] = ft_mt[(i + 397) % 624] ^ (y >> 1);
        if (y % 2 != 0) {
            ft_mt[i] = ft_mt[i] ^ 2567483615;
        }
    }
}

unsigned int ft_rand() {
    if (ft_index == 0) {
        generate_numbers();
    }

    unsigned int y = ft_mt[ft_index];
    y = y ^ (y >> 11);
    y = y ^ ((y << 7) & 2636928640);
    y = y ^ ((y << 15) & 4022730752);
    y = y ^ (y >> 18);

    ft_index = (ft_index + 1) % 624;
    return y;
}