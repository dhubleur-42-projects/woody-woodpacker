#ifndef ENCRYPTION_UTILS
# define ENCRYPTION_UTILS

# include <stdint.h>
# include <stdlib.h>

void	sub_word(uint32_t *word);
void	word_left_shift(uint32_t *word);
void	left_shift(uint8_t *array, size_t len);
void	n_left_shift(uint8_t *array, size_t len, size_t n_times);
uint8_t	get_sub_byte(uint8_t byte);
void	sub_bytes(uint8_t *buyes, size_t len);
void	right_shift(uint8_t *array, size_t len);
int		am_i_little_endian(void);

#endif
