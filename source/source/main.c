/*
 * main.c
 *
 * Created: 2020/10/02 19:45:03
 * Author : Y.Tsurui
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "peripheral/dcc_poller.h"
#include "peripheral/func_port.h"
#include "peripheral/event_timer.h"

#include "app/delay.h"

#include "dcc/dcc_phy.h"
#include "dcc/dcc_logic.h"
#include "dcc/cv.h"
#include "dcc/cv_value.h"

#include "app/func_ctrl.h"


int main(void)
{
    uint16_t dcc_timer_count_tmp;
	
	init_dcc_poller();
	init_func_port();
	initEventTimer();
	
	load_cv();
	
	sei();
	
	while (1) {
		dcc_timer_count_tmp = get_dcc_timer();
		if (dcc_timer_count_tmp) {
			receive_dcc_pulse(dcc_timer_count_tmp);
		}
		
		if (get_dcc_packet_cache_filled()) dcc_exec();
		
		if (getEventTimer()) {
			clock_receiver_dcc_exec();
			clockReceiverFuncCtrl();
			clockReveicerAnalogChecker();
			
			if (CV29 & 0x04) {
				if (chk_analog_direction()) {
					funcCtrlAnalog(chk_analog_direction());
				}
			}
		}
	}
}

