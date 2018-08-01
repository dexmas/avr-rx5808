
#pragma once

#include <Arduino.h>

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__) || defined(__AVR_ATmega32A__)
#define TCCR2A TCCR2
#define TCCR2B TCCR2
#define COM2A1 COM21
#define COM2A0 COM20
#define OCR2A OCR2
#define TIMSK2 TIMSK
#define OCIE2A OCIE2
#define TIMER2_COMPA_vect TIMER2_COMP_vect
#define TIMSK1 TIMSK
#endif

typedef struct 
{
	volatile int scanLine;
	volatile unsigned long frames;
	unsigned char start_render;
	int lines_frame;	  	//remove me
	uint8_t vres;
	uint8_t hres;
	uint8_t output_delay; 	//remove me
	char vscale_const;		//combine me with status switch
	char vscale;			//combine me too.
	char vsync_end;			//remove me
	uint8_t * screen;
    uint8_t enable_genlock;
    uint8_t clock_source;   // 0=intenr 1=extern
    uint8_t video_mode;     // keeps current video mode
    void (*vsync_handle)();   // must be triggered on edge of vsync
} TVout_vid;

extern TVout_vid display;

extern void (*hbi_hook)();
extern void (*vbi_hook)();

// genlock and video clock functions
#define CLOCK_INTERN            0
#define CLOCK_EXTERN            1

void start_internal_clock();
void start_extermal_clock();
void select_clock(uint8_t mode);
void vertical_handle();
void render_setup(uint8_t mode, uint8_t x, uint8_t y, uint8_t *scrnptr);
void blank_line();
void active_line();
void vsync_line();
void empty();

//tone generation properties
extern volatile long remainingToneVsyncs;

// 6cycles functions
void render_line6c();
void render_line5c();
void render_line4c();
void render_line3c();

static void inline wait_until(uint8_t time);

