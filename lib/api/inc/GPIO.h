#ifndef __GPIO_H
#define __GPIO_H

/* includes ---------------------------------------------------------------- */
#include "Common.h"

/* class ------------------------------------------------------------------- */
class GPIO
{
	protected:
		
		GPIO_TypeDef* m_port;
		uint32_t m_mask;
		uint8_t m_pin;
	
	public:
		
		static void clock(uint32_t port);
	
		GPIO(PinName pin, PinMode mode);
		void mode(PinMode mode);
		void type(PinType type);
		void pull(PinPull pull);
		void speed(PinSpeed speed);
		void af(PinAF f);
	
		uint32_t port(void);
		uint32_t mask(void);
		uint8_t pin(void);
	
		void write(uint32_t value);
		uint32_t read(void);
		operator uint32_t(); // Read (shorthand)
};

#endif
