#ifndef __RTC_H
#define __RTC_H

/* includes ---------------------------------------------------------------- */
#include "Common.h"

/* defines ----------------------------------------------------------------- */
#define RTC_BKP_VALUE 0x32F0
#define RTC_TIMEOUT_VALUE ((uint32_t)6000) // 48MHz / 40kHz = 1200 (x5 = 6000)
#define RTC_PREDIV_A ((uint32_t)127)
#define RTC_PREDIV_S ((uint32_t)311)

/* structures -------------------------------------------------------------- */
struct Time
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
};

struct Date
{
	uint8_t year;
	uint8_t month;
	uint8_t day;
};

/* class ------------------------------------------------------------------- */
class Clock
{
	private:
	
	public:
		
		Clock(void);
		void set(Time* time);
		void set(Date* date);
		void get(Time* time);
		void get(Date* date);
};

#endif
