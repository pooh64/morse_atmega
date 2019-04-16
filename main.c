#include "morse.h"
#include "led_matrix.h"
#include "timer_event.h"
#include <util/atomic.h>

int main()
{
	/* Make struct and clean it */
	struct morse_decoder dec = { };
	dec.symb = '_';
	led_matrix_t mat = { };
	led_matrix_set_from_symb(&mat, dec.symb);

	/* Prepare timer handler */
	timer_event_enable();

	while (1) {
		state_t tmp_sig;
		/* Atomic swap behaviour */
		ATOMIC_BLOCK(ATOMIC_FORCEON) {
			tmp_sig = timer_event_signal_state;
			timer_event_signal_state = STATE_NO_VALUE;
		}
		/* If there is new signal */
		if (tmp_sig != STATE_NO_VALUE) {
			uint8_t tmp_symb = dec.symb;
			morse_add_signal(&dec, tmp_sig);
			/* If there is a new symbol */
			if (dec.symb != tmp_symb) {
				led_matrix_set_from_symb(&mat, dec.symb);
			}
		}
		/* Redraw */
		led_matrix_draw(&mat);
	}
}
