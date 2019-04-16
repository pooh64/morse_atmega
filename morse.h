#ifndef MORSE_H_
#define MORSE_H_

#include <stdint.h>
#include <avr/io.h>
#include "state_type.h"

/* Lenghts measured in timer ticks */
#define SIGNAL_MIN_STABLE_LEN 2
#define MORSE_DOT_LEN 5

/* Dot */
#define MORSE_DASH_MIN (MORSE_DOT_LEN * 2)
/* Dash */
#define MORSE_DASH_MAX (MORSE_DOT_LEN * 4)
/* Error */

/* between units */
#define MORSE_SYMB_SPACE_MIN (MORSE_DOT_LEN * 2)
/* between symbols */
#define MORSE_SYMB_SPACE_MAX (MORSE_DOT_LEN * 5)
/* between words */

/* Decoder:
 *
 * + and - have ~10 sstate_len
 *
 * [+][-][+++][-][+++][---]  Signal state buffer ([ ] => flushed)
 *  .  s   -   s   -   nxt   Unit buffer
 *
 * --[+]--       => set unit .
 * --[+++]--     => set unit -
 * ++[-]++       => nothing
 * ++[---]++     => flush unit buffer
 * ++[-------]++ => space
*/

struct morse_decoder {
	struct signal_buffer {
		state_t state;
		uint8_t stable_len;
		uint8_t noise_len;
	} sig;

	struct unit_buffer {
		uint8_t buf;
		uint8_t len;
	} morse;

	uint8_t symb;
};

void morse_add_signal(struct morse_decoder *dec, state_t sig);
void morse_flush_units(struct morse_decoder *dec);

#endif /* MORSE_H_ */
