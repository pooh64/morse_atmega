#ifndef TIMER_EVENT_H_
#define TIMER_EVENT_H_

#include "state_type.h"
#include <avr/io.h>

#define BUTTON_DIR_REG  DDRD
#define BUTTON_PIN_REG  PIND
#define BUTTON_PORT_REG PORTD
#define BUTTON_BITMASK ((uint8_t) 0b10000000)

volatile extern state_t timer_event_signal_state;
void timer_event_enable();

#endif /* TIMER_EVENT_H */
