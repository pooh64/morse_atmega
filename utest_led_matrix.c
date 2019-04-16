#include "led_matrix.h"

int main()
{
	led_matrix_t mat = { };

	#define LED_MATRIX_SYMB(symbol, s0, s1, s2, s3, s4, s5, s6)	symbol,
	uint8_t ready_symbols[] = {
		#include "led_matrix_font.h"
		0 };
	#undef LED_MATRIX_SYMB

	while (1) {
		for (uint8_t *ptr = ready_symbols; *ptr != 0; ptr++) {
			led_matrix_set_from_symb(&mat, *ptr);
			for (uint16_t n = 128; n != 0; --n)
				led_matrix_draw(&mat);
		}
	}
}
