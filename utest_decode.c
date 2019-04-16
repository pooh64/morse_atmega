#include "morse.h"
#include "led_matrix.h"

void test_decode_cycle(struct morse_decoder *dec)
{
	led_matrix_t mat = { };
	morse_flush_units(dec);
	led_matrix_set_from_symb(&mat, dec->symb);

	for (uint16_t n = 128; n != 0; --n)
		led_matrix_draw(&mat);
	
	led_matrix_set_from_symb(&mat, 0);
	for (uint16_t n = 128; n != 0; --n)
		led_matrix_draw(&mat);
}

int main()
{
	struct morse_decoder dec = { };

	#define MORSE_SYMBOL_CODE(symbol, code_mask, len_mask)	\
		((uint8_t) code_mask),
	uint8_t ready_code_mask[] = {
		#include "morse_symbols.h"
		0 };
	#undef MORSE_SYMBOL_CODE

	#define MORSE_SYMBOL_CODE(symbol, code_mask, len_mask)	\
		((uint8_t) len_mask),
	uint8_t ready_len_mask[] = {
		#include "morse_symbols.h"
		0 };
	#undef MORSE_SYMBOL_CODE

	while (1) {
		for (uint8_t i = 0; i < sizeof(ready_code_mask) / sizeof(uint8_t); i++) {
			dec.morse.buf = ready_code_mask[i];
			dec.morse.len = ready_len_mask[i];
			test_decode_cycle(&dec);
		}
	}
}
