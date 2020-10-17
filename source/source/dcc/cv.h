/*
 * cv.h
 *
 * Created: 2020/10/02 19:57:39
 *  Author: ytsurui
 */ 

uint8_t read_cv(uint16_t);
void load_cv(void);
void write_cv_byte(uint16_t, uint8_t);
void write_cv_bit(uint16_t, uint8_t);
