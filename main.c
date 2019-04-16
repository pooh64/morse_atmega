#include "morse.h"

int main()
{
	/* Make struct and clean it */
	struct morse_decoder dec = { };
	dec.sym = '_';

	/* Prepare timer handler */
	timer_event_enable();

	while (1) {
		/* If there is new signal */
		state_t tmp_sig = timer_event_signal_state;
		if (tmp_sig != STATE_NO_VALUE) {
			uint8_t tmp_sym = dec.sym;
			morse_add_signal(&dec, tmp_sig);
			/* If there is a new symbol */
			if (dec.sym != tmp_sym) {
				morse_get_sym_matrix(&dec);
			}
		}
		/* Redraw */
		morse_draw_sym_matrix(&dec);
	}
}
