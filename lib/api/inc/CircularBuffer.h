#ifndef __CIRCULARBUFFER_H
#define __CIRCULARBUFFER_H

/* includes ----------------------------------------------------------------- */
#include "GPIO.h"

/* class -------------------------------------------------------------------- */
class CircularBuffer
{
	private:
		
		__IO uint16_t m_read;
		__IO uint16_t m_write; 
		__IO uint16_t m_count;
		__IO uint16_t m_size;
		__IO uint8_t* m_buffer;
	
	public:
		
		// !important: buffer size shall be a power of 2 (32, 64, 128, 256, ...)
		CircularBuffer(uint8_t* buffer, uint16_t size);
		
		uint16_t count(void);
		void flush(void);
		void put(uint8_t c);
		uint8_t get(void);
};

#endif /* __CIRCULARBUFFER_H */
