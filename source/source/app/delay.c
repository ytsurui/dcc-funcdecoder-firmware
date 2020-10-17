/*
 * delay.c
 *
 * Created: 2016/12/15 23:41:30
 * Project: motordecoder_simple_v1
 * Version: 1.00
 *	Target:	ATtiny45
 *	Author: Y.Tsurui
 */ 

#include <avr/io.h>
#include <util/delay.h>

void delay_ms(uint8_t ms)
{
	while (ms--) _delay_ms(1);
}
