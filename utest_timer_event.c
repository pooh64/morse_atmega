#include "led_matrix.h"
#include "timer_event.h"
#include <util/atomic.h>

int main()
{
	led_matrix_t mat = { };
	led_matrix_set_from_symb(&mat, '_');

	/* Prepare timer handler */
	timer_event_enable();

	while (1) {
		state_t tmp_sig;
		/* Atomic swap behavior */
		ATOMIC_BLOCK(ATOMIC_FORCEON) {
			tmp_sig = timer_event_signal_state;
			timer_event_signal_state = STATE_NO_VALUE;
		}

		if 	(tmp_sig == STATE_NO_VALUE)
			led_matrix_set_from_symb(&mat, 'a');
		else if (tmp_sig == STATE_ON)
			led_matrix_set_from_symb(&mat, 'b');
		else if (tmp_sig == STATE_OFF)
			led_matrix_set_from_symb(&mat, 'c');
		else
			led_matrix_set_from_symb(&mat, 'd');

		/* Redraw */
		for (uint8_t i = 35; i != 0; --i)
			led_matrix_draw(&mat);
	}
}
