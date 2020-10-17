/*
 * cv_value.h
 *
 * Created: 2020/10/02 20:00:39
 *  Author: Y.Tsurui
 */ 


#define CV_COUNT	255

#define HARDWARE_NUM	0x01
#define MANUFACTURENUM	0x0D

#define CV1_DEFAULT		3		//7-Bit Addr
#define CV17_DEFAULT	192		//14-Bit Addr
#define CV18_DEFAULT	0		//14-Bit Addr
#define CV29_DEFAULT	0x06	//Basic Configuration

#define CV33_DEFAULT	4				// F4 Headlight Dimming
#define CV34_DEFAULT	30				// Disable 
#define CV35_DEFAULT	0b01000000		// F0 Forward
#define CV36_DEFAULT	0b10000000		// F0 Backward
#define CV37_DEFAULT	0b00000011		// F3

#define CV45_DEFAULT	0b00000110		// F6 Headlight On

#define CV112_DEFAULT	0b01000000		// Headlight Dimming illuminate value
#define CV114_DEFAULT	0b11110001		// Func1 Type (Headlight)
#define CV115_DEFAULT	0b11110010		// Func2 Type (Taillight right)
#define CV116_DEFAULT	0b11110000		// Func3 Type (Normal Function)

uint8_t CV1;		// 8-bit Addr
//uint8_t CV2;
//uint8_t CV3;
//uint8_t CV4;
//uint8_t CV5;
//uint8_t CV6;
uint8_t CV17;		// 14-Bit Addr MSB
uint8_t CV18;		// 14-Bit Addr LSB
uint8_t CV19;		// Consist Addr
uint8_t CV29;		// Decoder Configuration
//uint8_t CV33;		// Headlight Dimming
//uint8_t CV34;		// Taillight Configuration
//uint8_t CV35;		// Func1 Address Configuration		
//uint8_t CV36;		// Func2 Address Configuration
//uint8_t CV37;		// Func3 Address Configuration

uint8_t CV33_37[5];

uint8_t CV45;		// Headlight Only Function


//uint8_t CV112;		// Headlight illuminate Value (bit7-bit4)
//uint8_t CV114;		// Func1 Type Configuration
//uint8_t CV115;		// Func2 Type Configuration
//uint8_t CV116;		// Func3 Type Configuration

uint8_t CV112_CV116[5];


uint8_t funcTable1;
uint8_t funcTable2;
uint8_t funcTable3;
uint8_t funcTable4;
uint8_t funcTable5;


/*	
 *	Function Configurations
 *		
 *		bit7-6: Direction Control
 *					00:	  Always
 *					01:	  Forward Only
 *					10:	  Backward Only
 *		bit5: Analog Enabled
 *					0: Analog Enabled
 *					1: Analog Disabled
 *		bit4-bit0:	Function Number
 *					0-28: Function Num
 *					30:	  Function Disabled
 *					31:   Always Enabled
 *
 */

/*
 *	Function Type Configurations
 *
 *	Lower-bit (bit3-bit0)
 *		0000:	Normal Function
 *		0001:	Headlight
 *		0010:	Taillight right
 *		0011:	Taillight left
 *
 *	Higher-bit (bit7-bit4)
 *		Function illuminate Value (0x0-0xF)
 *
 */