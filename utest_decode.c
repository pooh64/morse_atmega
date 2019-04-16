#include "morse.h"

void test_decode_cycle(struct morse_decoder *dec)
{
	morse_flush_units(dec);
	morse_get_sym_matrix(dec);

	for (uint16_t n = 128; n != 0; --n)
		morse_draw_sym_matrix(dec);
}

int main()
{
	struct morse_decoder dec = { };

	#define MORSE_SYMBOL_CODE(symbol, code_mask, len_mask)	\
       		code_mask | len_mask,
	uint8_t ready_codes[] = {
		#include "morse_symbols.h"
		0 };
	#undef MORSE_SYMBOL_CODE

	while (1) {
		for (uint8_t *ptr = ready_codes; *ptr != 0; ptr++) {
			dec.morse.buf = *ptr & (uint8_t) 0xb11111000;
			dec.morse.len = *ptr & (uint8_t) 0xb00000111;
			test_decode_cycle(&dec);
		}
	}
}
