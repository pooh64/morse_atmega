#include "morse.h"
#include <avr/interrupt.h>

volatile state_t timer_event_signal_state = STATE_NO_VALUE;

ISR(TIMER2_OVF_vect, ISR_BLOCK)
{
	if (SIGNAL_INDICATOR)
		timer_event_signal_state = STATE_ON;
	else
		timer_event_signal_state = STATE_OFF;
}

