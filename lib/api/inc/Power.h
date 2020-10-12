#ifndef __POWER_H
#define __POWER_H

/* includes ---------------------------------------------------------------- */
#include "Common.h"

/* class ------------------------------------------------------------------- */
class Power
{
	private:
	
	public:
		
		Power(void);
		void stop(void);
		void standby(PinName pin);
};

#endif
