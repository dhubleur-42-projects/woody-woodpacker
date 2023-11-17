/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 13:31:38 by dhubleur          #+#    #+#             */
/*   Updated: 2023/11/17 15:53:06 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/time.h>
#include <stdio.h>

unsigned int get_seed_from_timestamp() {
    uint64_t timestamp;
    syscall(SYS_clock_gettime, CLOCK_REALTIME, &timestamp);
    return (unsigned int) (timestamp ^ (timestamp >> 32));
}

//Mersenne Twister PRNG algorithm (https://en.wikipedia.org/wiki/Mersenne_Twister)
static unsigned int mt[624];
static int index = 0;

void ft_srand(unsigned int seed) {
    mt[0] = seed;
    for (int i = 1; i < 624; i++) {
        mt[i] = 1812433253 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i;
    }
}

static void generate_numbers() {
    for (int i = 0; i < 624; i++) {
        unsigned int y = (mt[i] & 0x80000000) + (mt[(i + 1) % 624] & 0x7fffffff);
        mt[i] = mt[(i + 397) % 624] ^ (y >> 1);
        if (y % 2 != 0) {
            mt[i] = mt[i] ^ 2567483615;
        }
    }
}

int ft_rand() {
    if (index == 0) {
        generate_numbers();
    }

    unsigned int y = mt[index];
    y = y ^ (y >> 11);
    y = y ^ ((y << 7) & 2636928640);
    y = y ^ ((y << 15) & 4022730752);
    y = y ^ (y >> 18);

    index = (index + 1) % 624;
    return (int) y;
}