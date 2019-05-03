#include "led_matrix.h"
#include <util/delay_basic.h>

static inline void inline_short_memset(void *ptr, uint8_t val, uint8_t n)
{
	uint8_t *tmp = ptr;
	while (n--)
		*tmp++ = val;
}

void led_matrix_set_from_symb(led_matrix_t *mat, uint8_t symb)
{
	if (symb == mat->symb) /* Cached */
		return;
	mat->symb = symb;
	switch (symb) {
	#define LED_MATRIX_SYMB(_symbol, s0, s1, s2, s3, s4, s5, s6)		\
	case _symbol: 								\
		mat->arr[0] = s0;						\
		mat->arr[1] = s1;						\
		mat->arr[2] = s2;						\
		mat->arr[3] = s3;						\
		mat->arr[4] = s4;						\
		mat->arr[5] = s5;						\
		mat->arr[6] = s6;						\
		break;
	#include "led_matrix_font.h"
	#undef LED_MATRIX_SYMB
	default:
		inline_short_memset(mat->arr, 0xff, sizeof(mat->arr));
	}
}

void led_matrix_draw(led_matrix_t *mat)
{
	MORSE_MATRIX_ROW_DIR = 0xff;
	MORSE_MATRIX_COL_DIR = 0xff;

	uint8_t *ptr = &mat->arr[6];

	for (uint8_t i = 7; i != 0; --i, --ptr) {
		MORSE_MATRIX_ROW_REG = ~((uint8_t) 1 << (i - 1));
		MORSE_MATRIX_COL_REG = *ptr;
		_delay_loop_1(MORSE_MATRIX_DRAW_DELAY);
		MORSE_MATRIX_COL_REG = 0;
	}

	MORSE_MATRIX_ROW_REG = 0;
}
