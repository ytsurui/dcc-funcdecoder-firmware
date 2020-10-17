/*
 * func_ctrl.c
 *
 * Created: 2020/10/02 22:07:39
 * Project: FuncDecoder_tiny45
 *  Author: Y.Tsurui
 */ 

#include <avr/io.h>
#include "../peripheral/func_port.h"
#include "../dcc/cv_value.h"
#include "../dcc/dcc_logic.h"

void funcSet(uint8_t funcNum, uint8_t stat, uint8_t direction);
void funcSetVirtual(uint8_t funcID, uint8_t cv, uint8_t funcNum, uint8_t stat);
void funcSetPort(uint8_t funcPort, uint8_t cv, uint8_t funcNum, uint8_t stat, uint8_t direction);
void funcSetPort2(uint8_t funcPort, uint8_t count);

uint8_t funcVirtualStat;
uint8_t funcPortStat;


uint8_t funcCount = 0;
uint8_t funcCount2 = 0;
#define FUNC_COUNT_MAX	0x0F

void clockReceiverFuncCtrl(void)
{
	//uint8_t directionFlag;
	//directionFlag = readDirectionFlag();
	//funcSetPort(0, CV33, 0x1F, 1, directionFlag);
	//funcSetPort(1, CV34, 0x1F, 1, directionFlag);
	//funcSetPort(2, CV35, 0x1F, 1, directionFlag);
	
	uint8_t i, mask;
	
	/*
	mask = 0x01;
	for (i = 0; i < 3; i++) {
		if (funcPortStat & mask) {
			func_ctrl(i, 1);
		} else {
			func_ctrl(i, 0);
		}
		mask = mask << 1;
	}
	
	
	return;
	*/
	
	
	funcCount++;
	if (funcCount > FUNC_COUNT_MAX) funcCount = 0;
	
	mask = 0x01;
	for (i = 0; i < 3; i++) {
		if ((CV33_37[i + 2] & 0x1F) == 0x1F) {
			// Analog Function On
			if ((CV33_37[i + 2] & 0xC0) == 0) {
				funcPortStat |= (1 << i);
			} else if (CV33_37[i + 2] & 0x80) {
				// Backward Only
				if (readDirectionFlag() == 2) {
					funcPortStat |= (1 << i);
				} else {
					funcPortStat &= ~(1 << i);
				}
			} else if (CV33_37[i + 2] & 0x40) {
				// Forward Only
				if (readDirectionFlag() == 1) {
					funcPortStat |= (1 << i);
				} else {
					funcPortStat &= ~(1 << i);
				}
			}
		}
		
		if (funcPortStat & mask) {
			// Function Enable
			funcSetPort2(i, funcCount);
		} else {
			// Function Disable
			if (funcVirtualStat & 0x04) {
				// Override Headlight Always On
				funcSetPort2(i, funcCount);
			} else if (funcVirtualStat & 0x02) {
				// Override Taillight
				funcSetPort2(i, funcCount);
			} else {
				func_ctrl(i, 0);
			}
		}
		mask = mask << 1;
	}
	
	
	
}

void funcCtrlAnalog(uint8_t direction) {
	if (direction == 1) {
		funcPortStat = 0b00000110;
	} else {
		funcPortStat = 0b00000101;
	}
}

void funcCtrl(uint8_t group, uint8_t funcData, uint8_t direction)	// F13-F20, F21-F28
{
	uint8_t i, funcTable, min, max, mask;
	
	if (group == 1) {
		if (funcTable1 == 0) return;
		funcTable = funcTable1;
		min = 1;
		max = 4;
	} else if (group == 2) {
		if (funcTable2 == 0) return;
		funcTable = funcTable2;
		min = 5;
		max = 8;
	} else if (group == 3) {
		if (funcTable3 == 0) return;
		funcTable = funcTable3;
		min = 9;
		max = 12;
	} else if (group == 4) {
		if (funcTable4 == 0) return;
		funcTable = funcTable4;
		min = 13;
		max = 20;
	} else if (group == 5) {
		if (funcTable5 == 0) return;
		funcTable = funcTable5;
		min = 21;
		max = 28;
	} else {
		return;
	}
	
	if (readDirectionReverse()) {
		if (direction == 1) {
			direction = 2;
		} else {
			direction = 1;
		}
	}
	
	mask = 0x01;
	for (i = min; i <= max; i++) {
		if (funcTable & mask) funcSet(i, funcData & mask, direction);
		mask = mask << 1;
	}
	
	if (group == 1) {
		// F0
		if (funcTable1 & 0x10) funcSet(0, funcData & 0x10, direction);
	}
	
}

void funcSet(uint8_t funcNum, uint8_t stat, uint8_t direction)
{
	/*
	funcSetVirtual(0, CV33, funcNum, stat);
	funcSetVirtual(1, CV34, funcNum, stat);
	funcSetPort(0, CV35, funcNum, stat, direction);
	funcSetPort(1, CV36, funcNum, stat, direction);
	funcSetPort(2, CV37, funcNum, stat, direction);
	*/
	
	funcSetVirtual(0, CV33_37[0], funcNum, stat);
	funcSetVirtual(1, CV33_37[1], funcNum, stat);
	funcSetPort(0, CV33_37[2], funcNum, stat, direction);
	funcSetPort(1, CV33_37[3], funcNum, stat, direction);
	funcSetPort(2, CV33_37[4], funcNum, stat, direction);
	funcSetVirtual(2, CV45, funcNum, stat);
}

void funcSetVirtual(uint8_t funcID, uint8_t cv, uint8_t funcNum, uint8_t stat)
{
	if ((cv & 0x1F) == funcNum) {
		if (stat) {
			funcVirtualStat |= (1 << funcID);
		} else {
			funcVirtualStat &= ~(1 << funcID);
		}
	}
}

void funcSetPort(uint8_t funcPort, uint8_t cv, uint8_t funcNum, uint8_t stat, uint8_t direction)
{
	uint8_t stat2;
	
	if ((cv & 0x1F) == funcNum) {
		// Check Direction
		stat2 = 0;
		if ((cv & 0xC0) == 0) {
			if (stat) stat2 = 1;
		} else if ((cv & 0xC0) == 0x40) {
			if ((direction == 1) && stat) stat2 = 1;
		} else if ((cv & 0xC0) == 0x80) {
			if ((direction == 2) && stat) stat2 = 1;
		}
		
		if (stat2) {
			funcPortStat |= (1 << funcPort);
		} else {
			funcPortStat &= ~(1 << funcPort);
		}
	}
}

void funcSetPort2(uint8_t funcPort, uint8_t count)
{
	//uint8_t cvAddr, cvType;
	uint8_t cvType;
	uint8_t illuminateValue;
	
	/*
	if (count > 0x08) {
		func_ctrl(funcPort, 0);
	} else {
		func_ctrl(funcPort, 1);
	}
	return;
	*/
	
	if (funcPort == 0) {
		//cvAddr = CV35;
		//cvType = CV114;
		cvType = CV112_CV116[2];
	} else if (funcPort == 1) {
		//cvAddr = CV36;
		//cvType = CV115;
		cvType = CV112_CV116[3];
	} else if (funcPort == 2) {
		//cvAddr = CV37;
		//cvType = CV116;
		cvType = CV112_CV116[4];
	} else {
		return;
	}
	
	
	if ((cvType & 0x0F) == 0x01) {
		// Headlight
		if (funcVirtualStat & 0x04) {
			// Headlight Only
			illuminateValue = (CV112_CV116[0] & 0xF0) >> 4;
		} else if (funcVirtualStat & 0x01) {
			// Headlight Dimming
			illuminateValue = (CV112_CV116[0] & 0xF0) >> 4;
		} else {
			if (readSpdFlag()) {
				illuminateValue = (cvType & 0xF0) >> 4;
			} else {
				illuminateValue = (CV112_CV116[0] & 0xF0) >> 4;
			}
		}
	} else if (cvType & 0x02) {
		// Taillight 
		if (funcVirtualStat & 0x04) {
			// Headlight Only, always off
			func_ctrl(funcPort, 0);
			return;			
		}
		illuminateValue = (cvType & 0xF0) >> 4;
	} else {
		illuminateValue = (cvType & 0xF0) >> 4;
	}
	
	
	if (count > illuminateValue) {
		func_ctrl(funcPort, 0);
	} else {
		func_ctrl(funcPort, 1);
	}
}