/*
 * dcc_logic.h
 *
 * Created: 2020/10/02 19:49:54
 *  Author: Y.Tsurui
 */ 


void clear_dcc_packet_cache(void);
void put_dcc_packet_cache(uint8_t data);
void set_dcc_packet_cache_filled_flag(void);
uint8_t get_dcc_packet_cache_filled(void);

void dcc_exec(void);
void clock_receiver_dcc_exec(void);

uint8_t readDirectionFlag(void);
uint8_t readSpdFlag(void);
