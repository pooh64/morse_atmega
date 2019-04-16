#ifndef TIMER_EVENT_H_
#define TIMER_EVENT_H_

#include "state_type.h"
#include <avr/io.h>

#define SIGNAL_INDICATOR (PORTD && 0x80)

volatile extern state_t timer_event_signal_state;
void timer_event_enable();

#endif /* TIMER_EVENT_H */