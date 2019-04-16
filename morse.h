#ifndef MORSE_H_
#define MORSE_H_

#include <stdint.h>
#include <avr/io.h>

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

#define MORSE_MATRIX_ROW_REG    PORTC
#define MORSE_MATRIX_COL_REG    PORTA
#define MORSE_MATRIX_DRAW_DELAY 0xff

#define SIGNAL_INDICATOR (PORTD && 0x80)


typedef enum state_type {
	STATE_NO_VALUE = 0,
	STATE_OFF,
	STATE_ON
} state_t;

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

	uint8_t sym;
	uint8_t sym_matrix[7];
};

void morse_add_signal(struct morse_decoder *dec, state_t sig);
void morse_flush_units(struct morse_decoder *dec);

void morse_get_sym_matrix(struct morse_decoder *dec);
void morse_draw_sym_matrix(struct morse_decoder *dec);

/* Timer routine */
volatile extern state_t timer_event_signal_state;
void timer_event_enable();

#endif /* MORSE_H_ */
