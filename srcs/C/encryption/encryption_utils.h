#ifndef ENCRYPTION_UTILS
# define ENCRYPTION_UTILS

# include <stdint.h>
# include <stdlib.h>

void	shift_rows(uint8_t *box);
void	mix_columns(uint8_t *box);
void	generate_keys(uint8_t *initial_key, uint8_t *subkeys);

void	sub_word(uint32_t *word);
void	word_left_shift(uint32_t *word);
void	left_shift(uint8_t *array, size_t len);
void	n_left_shift(uint8_t *array, size_t len, size_t n_times);
uint8_t	get_sub_byte(uint8_t byte);
void	sub_bytes(uint8_t *buyes, size_t len);
void	right_shift(uint8_t *array, size_t len);
int		am_i_little_endian(void);
void	up_shift(uint8_t *array, size_t width, size_t height, size_t i_column);
void	n_up_shift(uint8_t *array, size_t width, size_t height, size_t i_column, size_t n_times);

#endif
