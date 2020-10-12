#ifndef __BUTTON_H
#define __BUTTON_H

/* includes ---------------------------------------------------------------- */
#include "Digital.h"

/* defines ----------------------------------------------------------------- */
#define SHORT_PRESS 			0x01
#define LONG_PRESS				0x02

#define WAIT_PRESS 				0x01
#define WAIT_RELEASE 			0x02

#define DEBOUNCE_TIME					50	// 50 ms
#define LONG_PRESS_TIME 			900	// 900 ms

/* class ------------------------------------------------------------------- */
class Button : public DigitalOut
{
	private:
		
		uint8_t m_value;
		uint8_t m_state;
		uint16_t m_timer;
	
	public:
		
		Button(PinName pin);
		void update(void);
		uint8_t get(void);
		void timer(void);
};

#endif
