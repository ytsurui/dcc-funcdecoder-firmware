/*
 * cv.c
 *
 * Created: 2020/10/02 19:57:31
 * Project: FuncDecoder_tiny45
 *  Author: Y.Tsurui
 */ 

#include <avr/io.h>

#include "cv.h"
#include "cv_value.h"
#include "cv_eeprom.h"


uint8_t read_cv_raw(uint8_t CVnum)
{
	return (eeprom_read_byte(val + CVnum));
}

uint8_t read_cv(uint16_t CVnum)
{
	if (CVnum > CV_COUNT) return (0);
	
	return (read_cv_raw(CVnum & 0x00FF));
}

void write_eeprom(uint8_t addr, uint8_t data)
{
	eeprom_busy_wait();
	//eeprom_write_byte(val + addr, data);
	eeprom_update_byte(val + addr, data);
}

void initialize_cv(void)
{
	write_eeprom(1, CV1_DEFAULT);
	write_eeprom(7, HARDWARE_NUM);
	write_eeprom(8, MANUFACTURENUM);
	write_eeprom(17, CV17_DEFAULT);
	write_eeprom(18, CV18_DEFAULT);
	write_eeprom(19, 0);
	write_eeprom(29, CV29_DEFAULT);
	write_eeprom(33, CV33_DEFAULT);
	write_eeprom(34, CV34_DEFAULT);
	write_eeprom(35, CV35_DEFAULT);
	write_eeprom(36, CV36_DEFAULT);
	write_eeprom(37, CV37_DEFAULT);
	write_eeprom(45, CV45_DEFAULT);
	write_eeprom(112, CV112_DEFAULT);
	write_eeprom(114, CV114_DEFAULT);
	write_eeprom(115, CV115_DEFAULT);
	write_eeprom(116, CV116_DEFAULT);
	
	write_eeprom(0, 0);
	
	load_cv();
}

void initFuncTable(void) {
	funcTable1 = 0;
	funcTable2 = 0;
	funcTable3 = 0;
	funcTable4 = 0;
	funcTable5 = 0;
}

void setFuncTable(uint8_t funcNum) {
	if (funcNum <= 4) {
		if (funcNum == 0) {
			funcTable1 |= 0x10;
		} else {
			funcTable1 |= (1 << (funcNum - 1));
		}
	} else if (funcNum <= 8) {
		funcTable2 |= (1 << (funcNum - 5));
	} else if (funcNum <= 12) {
		funcTable3 |= (1 << (funcNum - 9));
	} else if (funcNum <= 20) {
		funcTable4 |= (1 << (funcNum - 13));
	} else if (funcNum <= 28) {
		funcTable5 |= (1 << (funcNum - 20));
	}
}


void load_cv(void)
{
	uint8_t CV0, i;

	eeprom_busy_wait();
	
	CV0 = read_cv_raw(0);
	
	if (CV0 == 0xFF) {
		//Initialize
		initialize_cv();
		return;
	}

	CV1 = read_cv_raw(1);
	//CV2 = read_cv_raw(2);
	//CV3 = read_cv_raw(3);
	//CV4 = read_cv_raw(4);
	//CV5 = read_cv_raw(5);
	//CV6 = read_cv_raw(6);

	/*
	if ((CV1 == 0xFF) && (CV2 == 0xFF) && (CV3 == 0xFF) && (CV4 == 0xFF) && (CV5 == 0xFF) & (CV6 == 0xFF)) {
		//Initialize
		initialize_cv();
		
		CV1 = read_cv_raw(1);
		CV2 = read_cv_raw(2);
		CV3 = read_cv_raw(3);
		CV4 = read_cv_raw(4);
		CV5 = read_cv_raw(5);
		CV6 = read_cv_raw(6);
	}
	*/
	CV17 = read_cv_raw(17);
	CV18 = read_cv_raw(18);
	CV19 = read_cv_raw(19);
	CV29 = read_cv_raw(29);
	/*
	CV33 = read_cv_raw(33);
	CV34 = read_cv_raw(34);
	CV35 = read_cv_raw(35);
	CV36 = read_cv_raw(36);
	CV37 = read_cv_raw(37);
	*/
	/*
	CV112 = read_cv_raw(112);
	CV114 = read_cv_raw(114);
	CV115 = read_cv_raw(115);
	CV116 = read_cv_raw(116);
	*/
	for (i = 0; i < 5; i++) {
		CV33_37[i] = read_cv_raw(33 + i);
		setFuncTable(CV33_37[i] & 0x1F);
		CV112_CV116[i] = read_cv_raw(112 + i);
	}
	CV45 = read_cv_raw(45);
	setFuncTable(CV45 & 0x1F);
	
	/*
	setFuncTable(CV33 & 0x1F);
	setFuncTable(CV34 & 0x1F);
	setFuncTable(CV35 & 0x1F);
	setFuncTable(CV36 & 0x1F);
	setFuncTable(CV37 & 0x1F);
	*/
}


void write_cv_byte(uint16_t CVnum, uint8_t data)
{
	uint8_t CV15, CV16, CVnum2;
	
	if (CVnum > CV_COUNT) return;
	
	CVnum2 = CVnum & 0x00FF;
	
	if ((CVnum2 != 15) && (CVnum2 != 16)) {
		CV15 = read_cv_raw(15);
		CV16 = read_cv_raw(16);
		
		if (CV15 != CV16) return;
	}

	switch (CVnum2) {
		case 1:
			CV1 = data;
			break;
		case 7: //CV7 Read-Only
			return;
		case 8: //CV8 Read-Only
			if ((data == MANUFACTURENUM) || (data == 8)) {
				initialize_cv();
			}
			return;
		case 17:
			CV17 = data;
			break;
		case 18:
			CV18 = data;
			break;
		case 19:
			CV19 = data;
			break;
		case 29:
			CV29 = data;
			break;
/*
		case 33:
			CV33 = data;
			setFuncTable(CV33 & 0x1F);
			break;
		case 34:
			CV34 = data;
			setFuncTable(CV34 & 0x1F);
			break;
		case 35:
			CV35 = data;
			setFuncTable(CV35 & 0x1F);
			break;
		case 36:
			CV36 = data;
			setFuncTable(CV36 & 0x1F);
			break;
		case 37:
			CV37 = data;
			setFuncTable(CV37 & 0x1F);
			break;
*/
		case 33:
		case 34:
		case 35:
		case 36:
		case 37:
			CV33_37[CVnum2 - 33] = data;
			setFuncTable(CV33_37[CVnum2 - 33] & 0x1F);
			break;
		case 45:
			CV45 = data;
			setFuncTable(CV45 & 0x1F);
			break;
		/*
		case 112:
			CV112 = data;
			break;
		case 114:
			CV114 = data;
			break;
		case 115:
			CV115 = data;
			break;
		case 116:
			CV116 = data;
			break;
		*/
		case 112:
		case 113:
		case 114:
		case 115:
		case 116:
			CV112_CV116[CVnum2 - 112] = data;
			break;
	}
	/*
	eeprom_busy_wait();
	eeprom_write_byte(CVnum2 - 1, data);
	eeprom_busy_wait();
	*/
	write_eeprom(CVnum2, data);
}

void write_cv_bit(uint16_t CVnum, uint8_t data)
{
	uint8_t int_t1, int_t2;
	
	int_t1 = data & 0x07;
	int_t2 = 1 << int_t1;
	int_t1 = read_cv(CVnum);

	if (data & 0x08) {
		//Bit = 1
		int_t1 |= int_t2;
	} else {
		//Bit = 0
		int_t1 &= ~int_t2;
	}

	write_cv_byte(CVnum, int_t1);
}




