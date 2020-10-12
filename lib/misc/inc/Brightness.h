#ifndef __BRIGHTNESS_H
#define __BRIGHTNESS_H

/* includes ---------------------------------------------------------------- */
#include "Timer.h"
#include "cie1931.h"

/* defines ----------------------------------------------------------------- */
#define BRIGHTNESS_MIN 0   // 0%
#define BRIGHTNESS_MAX 100 // 100%

/* structure --------------------------------------------------------------- */

/* class ------------------------------------------------------------------- */
class Brightness : public PwmOut
{
	private:
		
		uint8_t m_value;
	
	public:
		
		Brightness(PinName pin, uint32_t frequency, TIM_TypeDef* timer, TimerChannel channel);
		void set(uint8_t value);
		void set(void);
		uint8_t* value(void);
};

#endif
