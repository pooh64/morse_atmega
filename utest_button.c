#include "led_matrix.h"
#include "timer_event.h"
#include <util/atomic.h>

int main()
{
	led_matrix_t mat = { };
	led_matrix_set_from_symb(&mat, '_');

	DDRD  = 0x00;
	PORTD = 0xff;
	uint8_t mask = 0b10000000;

	while (1) {
		if 	(!(PIND & mask))
			led_matrix_set_from_symb(&mat, 'a');
		else
			led_matrix_set_from_symb(&mat, 'b');

		/* Redraw */
		for (uint8_t i = 35; i != 0; --i)
			led_matrix_draw(&mat);
	}
}
