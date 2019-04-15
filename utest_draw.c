#include "morse.h"

void test_draw_cycle(struct morse_decoder *dec)
{
	morse_get_sym_matrix(dec);

	for (uint16_t n = 128; n != 0; --n)
		morse_draw_sym_matrix(dec);
}

int main()
{
	struct morse_decoder dec = { };

	#define MORSE_SYMBOL_MATRIX(symbol, s0, s1, s2, s3, s4, s5, s6)	symbol,
	uint8_t ready_symbols[] = {
		#include "morse_symbols.h"
		0 };
	#undef MORSE_SYMBOL_MATRIX

	while (1) {
		for (uint8_t *ptr = ready_symbols; *ptr != 0; ptr++) {
			dec.sym = *ptr;
			test_draw_cycle(&dec);
		}
	}
}
