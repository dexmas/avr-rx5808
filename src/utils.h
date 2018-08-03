#pragma once

#include <avr/io.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

long map(long x, long in_min, long in_max, long out_min, long out_max);

void yield();

void init_timer();

unsigned long micros();
unsigned long millis();

void delay(unsigned long ms);
void delay_micro(unsigned int us);

int read_adc(uint8_t pin);