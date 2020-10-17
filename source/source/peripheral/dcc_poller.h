/*
 * dcc_poller.h
 *
 * Created: 2017/02/04 17:27:11
 * Project: motordecoder_simple_v1
 * Version: 1.00
 *	Target:	ATtiny45
 *	Author: Y.Tsurui
 */ 

/*
 *	Using PB0, Timer0
 */

#define DCC_INPUT_PORT_PORT		PORTB
#define DCC_INPUT_PORT_PIN		PINB
#define DCC_INPUT_PORT_DDR		DDRB

#define DCC_INPUT_PIN_NUM		PB0
#define DCC_INPUT_PIN_MASK		(1 << DCC_INPUT_PIN_NUM)

void init_dcc_poller(void);
uint16_t get_dcc_timer(void);

void clear_analog_flag(void);
uint8_t chk_analog_direction(void);
void clockReveicerAnalogChecker(void);
