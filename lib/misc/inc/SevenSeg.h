#ifndef __SEVENSEG_H
#define __SEVENSEG_H

/* includes ---------------------------------------------------------------- */
#include "Digital.h"

/* defines ----------------------------------------------------------------- */
#define MAX_SEGMENTS 7
#define MAX_DIGITS	 10
#define REFRESH_TIME 20 // 20 ms => 50 fps

/* structure --------------------------------------------------------------- */
struct Effect
{
	uint32_t reload;
	uint32_t timer;
	uint8_t flag;
	
	uint32_t select;
	uint8_t *buffer;
	uint32_t length;
	uint8_t shift;
};

/* variables --------------------------------------------------------------- */
//const uint8_t loop[8] = {0x40, 0x02, 0x01, 0x20, 0x40, 0x04, 0x08, 0x10}; 
//const uint8_t mad[8] = {0x3F, 0x7D, 0x7E, 0x5F, 0x3F, 0x7B, 0x77, 0x6F}; 
//const uint8_t wave[6] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20};
//const uint8_t dash[3] = {0x08, 0x40, 0x01};
//const uint8_t hex[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

/* class ------------------------------------------------------------------- */
class SevenSeg
{
	private:
		
		DigitalOut* m_segment[MAX_SEGMENTS];
		DigitalOut* m_digit;
	
	public:
		
		SevenSeg(DigitalOut* segment, DigitalOut* digit);
		void segments(uint8_t c);
		void set(uint8_t c);
		void set(uint32_t number);
		void on(void);
		void off(void);
};

class SevenSegGroup
{
	private:
		
		SevenSeg* m_digit[MAX_DIGITS];
		uint8_t m_buffer[MAX_DIGITS];
		uint8_t m_size;
		uint8_t m_inc;
		uint8_t m_mux;
		uint32_t m_timer;

		Effect m_blink;
		Effect m_scroll;
		Effect m_effect;
	
	public:
		
		SevenSegGroup(void);
		void update(void);
		void add(SevenSeg* digit);
		void set(uint8_t* buffer, uint32_t length);
		void set(int32_t number);
		void select(uint32_t segment);
		void blink(uint32_t ms);
		void blink(uint8_t select, uint32_t ms);
		void scroll(uint32_t ms);
		void effect(uint8_t* buffer, uint32_t length, uint32_t shift, uint32_t ms);
		void clear(void);
		void timer(void);
};

#endif
