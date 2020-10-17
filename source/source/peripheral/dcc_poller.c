/*
 * dcc_poller.c
 *
 * Created: 2017/02/04 17:26:59
 * Project: motordecoder_simple_v1
 * Version: 1.00
 *	Target:	ATtiny45
 *	Author: Y.Tsurui
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "dcc_poller.h"
#include "../dcc/dcc_phy.h"

uint8_t timer_msb = 0;
uint8_t analog_flag = 0;
uint8_t analog_pwm_flag = 0;

#define ANALOG_DETECT_VALUE		8

uint16_t dcc_timer_count_tmp;

void _dcc_timer_start(void);
uint16_t _dcc_timer_stop(void);

uint8_t analogCheckCounter = 0;
uint8_t analogDirectionFlag = 0;
uint8_t analogRightCount = 0;
uint8_t analogLeftCount = 0;

//DCC Pin-Change Interrupt
ISR(PCINT0_vect)
{
	//DCC Input Changed
	uint8_t sreg_tmp = SREG;
	
	if (analog_flag >= ANALOG_DETECT_VALUE) {
		analog_pwm_flag = 1;
		SREG = sreg_tmp;
		return;
	}

	if (DCC_INPUT_PORT_PIN & DCC_INPUT_PIN_MASK) {
		//DCC Input Pin Rising Edge
		_dcc_timer_start();
		dcc_timer_count_tmp = 0;
	} else {
		//DCC Input Pin Falling Edge
		dcc_timer_count_tmp = _dcc_timer_stop();
	}

	SREG = sreg_tmp;
}

//Timer Overflow Interrupt
ISR(TIMER0_OVF_vect)
{
	//Timer Overflow Interrupt
	uint8_t sreg_tmp = SREG;

	timer_msb++;
	if ((timer_msb == 0xFF) && (analog_flag != ANALOG_DETECT_VALUE)) analog_flag++;

	SREG = sreg_tmp;
}


void init_dcc_poller(void)
{
	//Set DCC Input Port
	DCC_INPUT_PORT_DDR &= ~DCC_INPUT_PIN_MASK;
	DCC_INPUT_PORT_PORT &= ~DCC_INPUT_PIN_MASK;
	
	//Set DCC Input Port PCINT
	GIMSK |= (1 << PCIE);
	PCMSK |= (1 << PCINT0);
	
	//Timer0 Configuration
	//	Normal Timer Mode, Prescaler = 8 (256us @ 8MHz, 1cycle = 1us)
	TCCR0A = (0 << COM0A1) | (0 << COM0A0) | (0 << COM0B1) | (0 << COM0B0) | (0 << WGM01) | (0 << WGM00);
	TCCR0B = (0 << FOC0A) | (0 << FOC0B) | (0 << WGM02) | (0 << CS02) | (1 << CS01) | (0 << CS00);
	TCNT0 = 0;
	
	//Timer0 1MHz Configuration
	//	Normal Timer Mode, Prescaler = 1 (256us @ 1MHz, 1cycle = 1us)
	//TCCR0A = (0 << COM0A1) | (0 << COM0A0) | (0 << COM0B1) | (0 << COM0B0) | (0 << WGM01) | (0 << WGM00);
	//TCCR0B = (0 << FOC0A) | (0 << FOC0B) | (0 << WGM02) | (0 << CS02) | (0 << CS01) | (1 << CS00);
	//TCNT0 = 0;
	
	
	//Timer0 Overflow Interrupt
	TIMSK |= (1 << TOIE0);
}


void _dcc_timer_start(void)
{
	TCNT0 = 0;
	timer_msb = 0;
}


uint16_t _dcc_timer_stop(void)
{
	uint16_t int_t;

	int_t = ((timer_msb << 8) + TCNT0);

	return (int_t);
}

uint16_t get_dcc_timer(void)
{
	uint16_t dcc_timer_tmp;

	if (dcc_timer_count_tmp != 0) {
		dcc_timer_tmp = dcc_timer_count_tmp;
		dcc_timer_count_tmp = 0;

		return (dcc_timer_tmp);
	}
	return (0);
}

void clear_analog_flag(void)
{
	analog_flag = 0;
	analog_pwm_flag = 0;
}


uint8_t chk_analog_direction(void)
{
	/*
	if (check_dcc_mac_analog_mode()) {
		// Backward
		return (1);
	}
	
	if (analog_flag < ANALOG_DETECT_VALUE) {
		return (0);
	}
	
	if ((DCC_INPUT_PORT_PIN & DCC_INPUT_PIN_MASK) || (analog_pwm_flag)) {
		// Forward
		return (2);
	} else {
		// Backward
		return (1);
	}
	*/
	if (analog_flag >= ANALOG_DETECT_VALUE) {
		return (analogDirectionFlag);
	}
	return (0);
}


void clockReveicerAnalogChecker(void)
{
	uint8_t diff;
	
	analogCheckCounter++;
	if (DCC_INPUT_PORT_PIN & DCC_INPUT_PIN_MASK) {
		if (ACSR & (1 << ACO)) {
			analogRightCount++;
			} else {
			analogLeftCount++;
		}
		} else {
		analogLeftCount++;
	}
	
	if (analogCheckCounter >= 0x1F) {
		//if (analogCheckCounter >= 0x0F) {
		if (analogRightCount > analogLeftCount) {
			diff = analogRightCount - analogLeftCount;
			if (diff > 13) analogDirectionFlag = 2;
			//if (diff > 7) analogDirectionFlag = 2;
			//analogDirectionFlag = 2;
			} else {
			diff = analogLeftCount - analogRightCount;
			if (diff > 9) analogDirectionFlag = 1;
			//if (diff > 3) analogDirectionFlag = 1;
			//analogDirectionFlag = 1;
		}
		analogCheckCounter = 0;
		analogRightCount = 0;
		analogLeftCount = 0;
	}
}

