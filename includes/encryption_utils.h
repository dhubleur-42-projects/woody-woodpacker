#ifndef ENCRYPTION_UTILS
# define ENCRYPTION_UTILS

# include <stdint.h>
# include <stdlib.h>

void	cipher(uint8_t *message, size_t len, uint8_t *key, uint8_t *iv, uint8_t *encrypted);
size_t	uncipher(uint8_t *encrypted, size_t len, uint8_t *key, uint8_t *iv, uint8_t *message);

void	shift_rows(uint8_t *box);
void	mix_columns(uint8_t *box);
void	generate_keys(uint8_t *initial_key, uint8_t *subkeys);
void	do_first_round(uint8_t *block, uint8_t *initial_key);
void	do_last_round(uint8_t *block, uint8_t *key);
void	do_regular_round(uint8_t *block, uint8_t *key);

void	inv_shift_rows(uint8_t *box);
void	inv_sub_bytes(uint8_t *bytes, size_t len);
void	inv_mix_columns(uint8_t *box);
void	reverse_first_round(uint8_t *block, uint8_t *initial_key);
void	reverse_last_round(uint8_t *block, uint8_t *key);
void	reverse_regular_round(uint8_t *block, uint8_t *key);

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
void	xor_bytes(uint8_t *a, uint8_t *b, size_t n_bytes);
void	add_padding(uint8_t *block, size_t block_len);

#endif
