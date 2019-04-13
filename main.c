#include <avr/io.h>
#include <avr/interrupt.h>

/* Timer interrupts */

typedef enum state_type {
	STATE_NO_VALUE = 0,
	STATE_OFF,
	STATE_ON
} state_t;

volatile state_t g_timer_event_sstate = STATE_NO_VALUE;
void timer_event_enable();

/* Something like that */
ISR(TIMER2_OVF_vect, ISR_BLOCK)
{
	/* Set g_timer_event_sstate */
}


/* Decode:
 *
 * + and - have ~10 sstate_len
 *
 * [+][-][+++][-][+++][---]  Signal state buffer ([ ] => flushed)
 *  .  s   -   s   -   nxt   Unit buffer
 *
 * --[+]--       => set unit .
 * --[+++]--     => set unit -
 * ++[-]++       => nothing
 * ++[---]++     => time to flush unit buffer
 * ++[-------]++ => space
*/

struct mdecode_buf {
	state_t sstate_noise;  	/* Ignoring noises */
	state_t sstate;	       	/* Signal state */
	uint8_t sstate_len;

	uint8_t ubuf;          	/* Unit buffer */
	uint8_t ubuf_len;

	char symb;		/* Symbol to draw */
};

int main()
{
	while (1) ;
}
