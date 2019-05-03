#include "timer_event.h"
#include <avr/interrupt.h>

volatile state_t timer_event_signal_state = STATE_NO_VALUE;

void timer_event_enable()
{
	TCNT0 = 0;
	TCCR0 =   ((uint8_t) 1 << CS02) | ((uint8_t) 1 << CS00)
		| ((uint8_t) 1 << WGM01);
	OCR0 = 254;
	TIMSK = (1 << OCIE0);

	BUTTON_DIR_REG  &= !BUTTON_BITMASK; /* Set to input   */
	BUTTON_PORT_REG |=  BUTTON_BITMASK; /* Set resistance */

	sei();
}

ISR(TIMER0_COMP_vect)
{
	if (!(BUTTON_PIN_REG & BUTTON_BITMASK))
		timer_event_signal_state = STATE_ON;
	else
		timer_event_signal_state = STATE_OFF;
}

