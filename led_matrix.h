#ifndef LED_MATRIX_H_
#define LED_MATRIX_H_

/* Module to print letters on 5x7 led matrix */

#include <avr/io.h>

#define MORSE_MATRIX_ROW_DIR	DDRC
#define MORSE_MATRIX_COL_DIR	DDRA
#define MORSE_MATRIX_ROW_REG	PORTC
#define MORSE_MATRIX_COL_REG	PORTA
#define MORSE_MATRIX_DRAW_DELAY 0x80 /* 0x01 = 28us */

typedef struct led_matrix {
	uint8_t arr[7];
	uint8_t symb;	/* Cache */
} led_matrix_t;

void led_matrix_set_from_symb(led_matrix_t *mat, uint8_t symb);
void led_matrix_draw(led_matrix_t *mat);

#endif /* LED_MATRIX_H_ */
