#include "morse.h"
#include <util/delay_basic.h>

static inline void set_bit(uint8_t *p, uint8_t num) {
	uint8_t mask = (uint8_t) 1 << num;
	*p |= mask;
}

static inline void clr_bit(uint8_t *p, uint8_t num) {
	uint8_t mask = (uint8_t) 1 << num;
	*p &= ~mask;
}

void morse_get_sym_matrix(struct morse_decoder *dec)
{
	uint8_t sym = dec->sym;
	switch (sym) {
	#define MORSE_SYMBOL_MATRIX(symbol, s0, s1, s2, s3, s4, s5, s6)	\
	case symbol: 							\
		dec->sym_matrix[0] = s0;				\
		dec->sym_matrix[1] = s1;				\
		dec->sym_matrix[2] = s2;				\
		dec->sym_matrix[3] = s3;				\
		dec->sym_matrix[4] = s4;				\
		dec->sym_matrix[5] = s5;				\
		dec->sym_matrix[6] = s6;				\
		break;
	#include "morse_symbols.h"
	#undef MORSE_SYMBOL_MATRIX
	default:
		dec->sym_matrix[0] = 0xff;
		dec->sym_matrix[1] = 0xff;
		dec->sym_matrix[2] = 0xff;
		dec->sym_matrix[3] = 0xff;
		dec->sym_matrix[4] = 0xff;
		dec->sym_matrix[5] = 0xff;
		dec->sym_matrix[6] = 0xff;
	}
}

void morse_draw_sym_matrix(struct morse_decoder *dec)
{
	DDRA = 0xff; /* We need macro here */
	DDRC = 0xff;

	uint8_t *ptr = dec->sym_matrix + 6;

	for (uint8_t i = 7; i != 0; --i, --ptr) {
		MORSE_MATRIX_ROW_REG = ~((uint8_t) 1 << (i - 1));
		MORSE_MATRIX_COL_REG = *ptr;
		_delay_loop_1(MORSE_MATRIX_DRAW_DELAY);
		MORSE_MATRIX_COL_REG = 0;
	}

	MORSE_MATRIX_ROW_REG = 0;

	/* Maybe restore DDR */
}

static inline void morse_flush_error(struct morse_decoder *dec)
{
	dec->morse.buf = 0;
	dec->morse.len = 0;
	dec->sym = '@';
}

static inline void morse_flush_space(struct morse_decoder *dec)
{
	dec->morse.buf = 0;
	dec->morse.len = 0;
	dec->sym = '_';
}

void morse_flush_units(struct morse_decoder *dec)
{
	uint8_t result;
	/* Magic trick */
	uint8_t code = dec->morse.buf | dec->morse.len;
	dec->morse.buf = 0;
	dec->morse.len = 0;

	switch (code) {
	#define MORSE_SYMBOL_CODE(symbol, code_mask, len_mask)	\
	case ((uint8_t) code_mask) | ((uint8_t) len_mask): 	\
		result = symbol; 				\
		break;
	#include "morse_symbols.h"
	#undef MORSE_SYMBOL_CODE
	default:
		result = '@';
	}

	dec->sym = result;
}

void morse_flush_signal(struct morse_decoder *dec)
{
	if (dec->sig.state == STATE_ON) {
		/* Too long signals */
		if (dec->morse.len > 4)
			morse_flush_error(dec);

		if (dec->sig.stable_len < MORSE_DASH_MIN)
			dec->morse.len++;
		else if (dec->sig.stable_len <= MORSE_DASH_MAX)
			set_bit(&dec->morse.buf, 8 - dec->morse.len++);
		else
			morse_flush_error(dec);
	} else {
		if (dec->sig.stable_len < MORSE_SYMB_SPACE_MIN)
			; /* Just space between units */
		else if (dec->sig.stable_len < MORSE_SYMB_SPACE_MAX)
			morse_flush_units(dec);
		else	/* It's space between words */
			morse_flush_space(dec);
	}
}

void morse_add_signal(struct morse_decoder *dec, state_t new_state)
{
	/* Just stable or short noise finished */
	if (new_state == dec->sig.state) {
		uint8_t add = dec->sig.noise_len + 1;
		dec->sig.noise_len = 0;

		/* Overflow protection */
		if (add < UINT8_MAX - dec->sig.stable_len)
			dec->sig.stable_len += add;
		else
			dec->sig.stable_len = UINT8_MAX;
		return;
	}

	dec->sig.noise_len++;

	/* It's too long for noise, change state and flush old signal */
	if (dec->sig.noise_len >= SIGNAL_MIN_STABLE_LEN) {
		dec->sig.noise_len = 0;

		/* Overflow protection */
		if (SIGNAL_MIN_STABLE_LEN / 2 < UINT8_MAX - dec->sig.stable_len)
			dec->sig.stable_len += SIGNAL_MIN_STABLE_LEN / 2;
		else
			dec->sig.stable_len = UINT8_MAX;

		morse_flush_signal(dec);

		dec->sig.state      = new_state;
		dec->sig.stable_len = SIGNAL_MIN_STABLE_LEN / 2;
	}
}
