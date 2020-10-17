/*
 * event_timer.c
 *
 * Created: 2020/10/02 22:18:20
 * Project: FuncDecoder_tiny45
 *  Author: Y.Tsurui
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "event_timer.h"


uint8_t eventTimerCounter;
//#define EVENT_TIMER_MAX		32		// For 8MHz
#define EVENT_TIMER_MAX			4		// For 8MHz (prescaler 8)
//#define EVENT_TIMER_MAX		2		// For 4MHz
//#define EVENT_TIMER_MAX		1		// For 4MHz


ISR(TIMER1_OVF_vect) {
	if (eventTimerCounter < EVENT_TIMER_MAX) {
		eventTimerCounter++;
	}
}

uint8_t getEventTimer(void)
{
	if (eventTimerCounter >= EVENT_TIMER_MAX) {
		eventTimerCounter = 0;
		return (1);
	}
	return (0);
}

void initEventTimer(void)
{
	
	//CLKPR |= 0x80;
	//CLKPR = 0x01;	// Prescaler = 1/2 (4MHz)
	//PLLCSR = (1 << PLLE);
	//while (PLLCSR & (1 << PLOCK));
	//PLLCSR = (1 << PLLE) | (1 << PCKE);
	
	GTCCR = (0 << PWM1B) | (0 << COM1B1) | (0 << COM1B0) | (0 << FOC1B) | (0 << FOC1A) | (0 << PSR1);
	// Fast PWM-Mode, Prescaler = 1/8 (No Prescaling, 32kHz @ 8MHz)
	TCCR1 = (0 << CTC1) | (1 << PWM1A) | (0 << COM1A1) | (0 << COM1A0) | (0 << CS13) | (1 << CS12) | (0 << CS11) | (0 << CS10);
	TIMSK &= ~(1 << OCIE1B);
	TIMSK |= (1 << TOIE1);
	
}