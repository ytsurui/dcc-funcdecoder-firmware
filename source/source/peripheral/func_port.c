/*
 * func_port.c
 *
 * Created: 2020/10/02 22:13:51
 * Project: FuncDecoder_tiny45
 * Version: 1.0.0
 *  Target: Attiny45
 *  Author: Y.Tsurui
 */ 

#include <avr/io.h>
#include "func_port.h"

void init_func_port(void)
{
	FUNC_PORT_DDR |= FUNC_PORT_DDR_MASK;
	FUNC_PORT_DDR &= ~DIR_REVERSE_JUMPER_MASK;
	FUNC_PORT_PORT &= ~FUNC_PORT_DDR_MASK;
	FUNC_PORT_PORT |= DIR_REVERSE_JUMPER_MASK;
}

void func_ctrl(uint8_t func_num, uint8_t func_stat)
{
	uint8_t funcPin = 0;
	
	switch (func_num) {
		case 0:
			funcPin = FUNC_PORT_PIN1_MASK;
			break;
		case 1:
			funcPin = FUNC_PORT_PIN2_MASK;
			break;
		case 2:
			funcPin = FUNC_PORT_PIN3_MASK;
			break;
		default:
			return;
	}
	
	if (func_stat) {
		FUNC_PORT_PORT |= funcPin;
	} else {
		FUNC_PORT_PORT &= ~funcPin;
	}
	
}

uint8_t readDirectionReverse(void)
{
	if (~PINB & DIR_REVERSE_JUMPER_MASK) {
		return (1);
	}
	return (0);
}