#ifndef __WATCHDOG_H
#define __WATCHDOG_H

/* includes ---------------------------------------------------------------- */
#include "Common.h"

/* defines ----------------------------------------------------------------- */
#define IWDG_KR_WRITE	((uint16_t)0x5555)
#define IWDG_KR_ENABLE	((uint16_t)0xCCCC)
#define IWDG_KR_RELOAD	((uint16_t)0xAAAA)

/* class ------------------------------------------------------------------- */
class Watchdog
{
	private:
	
	public:
		
		Watchdog(uint32_t ms);
		void reload(void);
};

#endif
