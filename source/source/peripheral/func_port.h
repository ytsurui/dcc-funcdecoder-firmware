/*
 * func_port.h
 *
 * Created: 2020/10/02 22:14:02
 * Project: funcdecoder_tiny45
 * Version: 1.00
 *	Target:	ATtiny45
 *	Author: Y.Tsurui
 */ 

#define FUNC_PORT_PORT			PORTB
#define FUNC_PORT_DDR			DDRB
#define FUNC_PORT_PIN			PINB

#define FUNC_PORT_PIN1			PB1		// Func1_For
#define FUNC_PORT_PIN2			PB2		// Func1_Rev
#define FUNC_PORT_PIN3			PB3		// Func2
#define DIR_REVERSE_JUMPER		PB4		// Direction Reverse Jumper

#define FUNC_PORT_PIN1_MASK		(1 << FUNC_PORT_PIN1)
#define FUNC_PORT_PIN2_MASK		(1 << FUNC_PORT_PIN2)
#define FUNC_PORT_PIN3_MASK		(1 << FUNC_PORT_PIN3)
#define DIR_REVERSE_JUMPER_MASK	(1 << DIR_REVERSE_JUMPER)

#define FUNC_PORT_DDR_MASK		FUNC_PORT_PIN1_MASK | FUNC_PORT_PIN2_MASK | FUNC_PORT_PIN3_MASK

void init_func_port();
void func_ctrl(uint8_t func_num, uint8_t func_stat);
uint8_t readDirectionReverse(void);
