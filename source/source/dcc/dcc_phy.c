/*
 * dcc_phy.c
 *
 * Created: 2020/10/02 19:49:05
 *  Author: Y.Tsurui
 */ 


#include <avr/io.h>

#include "dcc_logic.h"

/* BaseClock = 16MHz */
//#define DCC_PULSE_LENGTH_ONE_MIN	88		//44us (counter 1cycle = 0.5us @ 16MHz)
//#define DCC_PULSE_LENGTH_ONE_MAX	160		//80us
//#define DCC_PULSE_LENGTH_ZERO_MIN	180		//90us
//#define DCC_PULSE_LENGTH_ZERO_MAX	20000	//10000us

/* BaseClock = 8MHz */
#define DCC_PULSE_LENGTH_ONE_MIN	44		//44us (counter 1cycle = 1us @ 8MHz)
#define DCC_PULSE_LENGTH_ONE_MAX	80		//80us
#define DCC_PULSE_LENGTH_ZERO_MIN	90		//90us
//#define DCC_PULSE_LENGTH_ZERO_MAX	10000	//10000us
#define DCC_PULSE_LENGTH_ZERO_MAX	20000	//20000us

/* BaseClock = 4MHz */
//#define DCC_PULSE_LENGTH_ONE_MIN	20		//44us (counter 1cycle = 2us @ 4MHz)
//#define DCC_PULSE_LENGTH_ONE_MAX	42		//80us
//#define DCC_PULSE_LENGTH_ZERO_MIN	44		//90us
//#define DCC_PULSE_LENGTH_ZERO_MAX	10000	//10000us
//#define DCC_PULSE_LENGTH_ZERO_MAX	10000	//20000us

/* BaseClock = 2MHz */
//#define DCC_PULSE_LENGTH_ONE_MIN	11		//44us (counter 1cycle = 4us @ 2MHz)
//#define DCC_PULSE_LENGTH_ONE_MAX	20		//80us
//#define DCC_PULSE_LENGTH_ZERO_MIN	23		//90us
//#define DCC_PULSE_LENGTH_ZERO_MAX	10000	//10000us
//#define DCC_PULSE_LENGTH_ZERO_MAX	5000	//20000us


uint8_t _dcc_pulse_receive_bit;
uint8_t _preamble_count;

#define DCC_PACKET_LENGTH	8

uint8_t _dcc_pulse_bit_count;
uint8_t _dcc_packet_data[DCC_PACKET_LENGTH];
uint8_t _dcc_packet_count;
uint8_t _dcc_packet_error;

uint8_t pulse_check_analog_counter = 0;
#define PULSE_ANALOG_CNT_THRESHOLD		40

uint8_t packet_check_analog_counter = 0;
#define PACKET_ANALOG_CNT_THRESHOLD		40


void receive_dcc_pulse(uint16_t pulse_count)
{
	uint8_t i;

	if ((pulse_count >= DCC_PULSE_LENGTH_ONE_MIN) && (pulse_count <= DCC_PULSE_LENGTH_ONE_MAX)) {
		//Pulse Length = 1

		_dcc_pulse_receive_bit = 1;
		_preamble_count++;

		_dcc_packet_data[_dcc_packet_count] = (_dcc_packet_data[_dcc_packet_count] << 1) + 1;
		
		if (pulse_check_analog_counter > 0) pulse_check_analog_counter--;
		} else if ((pulse_count >= DCC_PULSE_LENGTH_ZERO_MIN) && (pulse_count <= DCC_PULSE_LENGTH_ZERO_MAX)) {
		//Pulse Length = 0

		_dcc_pulse_receive_bit = 0;
		
		if (_preamble_count >= 14) {
			//Packet Start Bit
			//Initialize DCC Variables
			_dcc_pulse_bit_count = 1;
			_dcc_packet_data[0] = 0;
			_dcc_packet_count = 0;
			_dcc_packet_error = 0;
			_preamble_count = 0;
			return;
		}

		_preamble_count = 0;

		_dcc_packet_data[_dcc_packet_count] = (_dcc_packet_data[_dcc_packet_count] << 1);
		
		if (pulse_check_analog_counter > 0) pulse_check_analog_counter--;
		} else {
		//Non-Standard Length Pulse
		//For Analog-PWM-Powerpack
		
		if (pulse_check_analog_counter < PULSE_ANALOG_CNT_THRESHOLD) {
			pulse_check_analog_counter++;
			if (pulse_check_analog_counter == PULSE_ANALOG_CNT_THRESHOLD) {
				pulse_check_analog_counter += 10;
			}
		}
		return;
	}

	if (_dcc_packet_count == 0xFF) {
		//Ignore Pulse
		return;
	}

	if ((_dcc_pulse_bit_count == 0) && (_dcc_pulse_receive_bit == 1)) {
		//Packet End Bit
		if (_dcc_packet_error == 0) {
			//Packet Error Check Complete

			clear_dcc_packet_cache();

			for (i = 0; i < _dcc_packet_count; i++) {
				put_dcc_packet_cache(_dcc_packet_data[i]);
			}

			set_dcc_packet_cache_filled_flag();

			_dcc_packet_count = 0xFF;
			
			if (packet_check_analog_counter > 0) packet_check_analog_counter--;
			} else {
			if (packet_check_analog_counter < PACKET_ANALOG_CNT_THRESHOLD) {
				packet_check_analog_counter++;
				if (packet_check_analog_counter == PACKET_ANALOG_CNT_THRESHOLD) {
					packet_check_analog_counter += 10;
				}
			}
		}
	}

	_dcc_pulse_bit_count++;


	if (_dcc_pulse_bit_count == 9) {
		//Byte End (1-8: Packet Pulse, 9 = end)

		if ((_dcc_packet_count == 0) && (_dcc_packet_data[0] == 0xFF)) {
			//Preamble
			_dcc_packet_count = 0xFF;
			return;
		}

		_dcc_packet_error ^= _dcc_packet_data[_dcc_packet_count];
		_dcc_packet_count++;
		_dcc_pulse_bit_count = 0;
	}
}

uint8_t check_dcc_mac_analog_mode(void)
{
	if ((pulse_check_analog_counter >= PULSE_ANALOG_CNT_THRESHOLD) || (packet_check_analog_counter >= PACKET_ANALOG_CNT_THRESHOLD)) {
		return (1);
	}
	return (0);
}