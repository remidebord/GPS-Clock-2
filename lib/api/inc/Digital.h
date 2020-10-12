#ifndef __DIGITAL_H
#define __DIGITAL_H

/* includes ---------------------------------------------------------------- */
#include "GPIO.h"

/* class ------------------------------------------------------------------- */
class DigitalOut : public GPIO
{
	protected:
	
	public:
		
		DigitalOut(PinName pin);
	
		DigitalOut& operator= (uint32_t value);	// Write (shorthand) !important: operator = cannot be inherited !
};

class DigitalIn : public GPIO
{
	protected:
	
	public:
		
		DigitalIn(PinName pin);
};

class InterruptIn : public GPIO
{
	protected:
		
	public:
		InterruptIn(PinName pin);
		void rise(void(*f)(void));
		void fall(void(*f)(void));
		void risefall(void(*f)(void));
};

#endif
