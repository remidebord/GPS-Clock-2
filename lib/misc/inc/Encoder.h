#ifndef __ENCODER_H
#define __ENCODER_H

/* includes ---------------------------------------------------------------- */
#include "Digital.h"

/* defines ----------------------------------------------------------------- */

/* class ------------------------------------------------------------------- */
class Encoder
{
	private:
		
		InterruptIn* m_A;
		InterruptIn* m_B;
		uint8_t m_value;
		uint8_t m_old;
	
		uint32_t m_accum;
	
		uint8_t* m_variable;
		uint8_t m_limit;
		uint32_t m_n, m_p;
		uint8_t m_circular;
	
	public:
		
		Encoder(InterruptIn* A, InterruptIn* B);
		void update(void);
		void circular(uint8_t x);
		void attach(uint8_t* variable, uint8_t limit);
		uint32_t get(void);
		void irq(void);
	
		static int32_t round(int32_t number, int32_t divisor);
};

#endif
