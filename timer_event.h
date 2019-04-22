#ifndef TIMER_EVENT_H_
#define TIMER_EVENT_H_

#include "state_type.h"
#include <avr/io.h>

#define SIGNAL_INDICATOR (PORTD & (uint8_t) 1 << 6)
//#define SIGNAL_INDICATOR (PORTD & (uint8_t) 1 << 7)
//#define SIGNAL_INDICATOR (PORTD)

volatile extern state_t timer_event_signal_state;
void timer_event_enable();

#endif /* TIMER_EVENT_H */
