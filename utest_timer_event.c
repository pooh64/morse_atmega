#include "led_matrix.h"
#include "timer_event.h"
#include <util/atomic.h>

int main()
{
	led_matrix_t mat = { };
	led_matrix_set_from_symb(&mat, '_');

	/* Prepare timer handler */
	timer_event_enable();

	uint8_t counter = 0;
	uint8_t overflow = 100;

	while (1) {
		state_t tmp_sig;
		/* Atomic swap behavior */
		led_matrix_set_from_symb(&mat, 'o');

		ATOMIC_BLOCK(ATOMIC_FORCEON) {
			tmp_sig = timer_event_signal_state;
			timer_event_signal_state = STATE_NO_VALUE;
		}

		if (tmp_sig == STATE_ON) {
			if ((++counter) >= overflow) {
				counter = 0;
				led_matrix_set_from_symb(&mat, 't');
			}
		}

		/* Redraw */
		for (uint8_t i = 1; i != 0; --i)
			led_matrix_draw(&mat);
	}
}
