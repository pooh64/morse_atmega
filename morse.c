#include "morse.h"

static inline void set_bit(uint8_t *p, uint8_t num) {
	uint8_t mask = (uint8_t) 1 << num;
	*p |= mask;
}

static inline void clr_bit(uint8_t *p, uint8_t num) {
	uint8_t mask = (uint8_t) 1 << num;
	*p &= ~mask;
}

static inline void morse_flush_error(struct morse_decoder *dec)
{
	dec->morse.buf = 0;
	dec->morse.buf_len = 0;
	dec->symbol = '@';
}

static inline void morse_flush_space(struct morse_decoder *dec)
{
	dec->morse.buf = 0;
	dec->morse.buf_len = 0;
	dec->symbol = '_';
}

void morse_flush_units(struct morse_decoder *dec)
{
	uint8_t result;
	uint8_t buf = dec->morse.buf;
	uint8_t len = dec->morse.len;
	dec->morse.buf = 0;
	dec->morse.len = 0;

	switch (buf) {
	#define MORSE_SYMBOL_CODE(code_mask, len_mask, symbol)	\
	case ((uint8_t) code_mask) | ((uint8_t) len_mask): 	\
		result = symbol; 				\
		break;
	#include "morse_symbols.h"
	#undef MORSE_SYMBOL_CODE
	defalut:
		result = '@';
	}

	dec->symbol = result;
	return;
}

void morse_flush_signal(struct morse_decoder *dec)
{
	if (state == STATE_ON) {
		if (dec->sig.stable_len <  MORSE_DASH_MIN) {
			/* clr_bit(&dec->morse.buf, dec->morse.buf_len++); */
			/* Just set buf to 0 by default */
		} else if (dec->sig.stable_len <= MORSE_DASH_MAX) {
			set_bit(&dec->morse.buf, 8 - dec->morse.buf_len++);
		} else
			morse_flush_error(dec);
		}
	} else {
		if (len < MORSE_SYMB_SPACE_MIN)
			; /* Just space between units */
		else if (len < MORSE_SYMB_SPACE_MAX)
			morse_flush_units(dec);
		else	/* It's space between words */
			morse_flush_space(dec);
	}

	return;
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

	/* It's too long for noise, change state and flush old */
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

	return;
}

