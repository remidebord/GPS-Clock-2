#ifndef __TIMEZONE_H
#define __TIMEZONE_H

/* includes ---------------------------------------------------------------- */
#include "RTC.h"
#include "GPS.h"
#include "Grid.h"

/* defines ----------------------------------------------------------------- */
//#define TILES_X 1414
//#define TILES_Y 707

//#define LATITUDE_MIN ((float)-90)
//#define LATITUDE_MAX ((float)90)
//#define LONGITUDE_MIN ((float)-180)
//#define LONGITUDE_MAX ((float)180)

//#define LATITUDE_RANGE ((float)LATITUDE_MAX-LATITUDE_MIN)
//#define LONGITUDE_RANGE ((float)LONGITUDE_MAX-LONGITUDE_MIN)

//#define HOUR_SEC 3600
//#define QUARTER_HOURS_SEC (HOUR_SEC/4)

#define TIMEZONE_COLUMNS 1414
#define TIMEZONE_ROWS    707

#define TIMEZONE_LAT_MIN ((float)-90)
#define TIMEZONE_LON_MIN ((float)-180)
#define TIMEZONE_LAT_MAX ((float)90)
#define TIMEZONE_LON_MAX ((float)180)

#define TIMEZONE_LAT_RANGE ((float)TIMEZONE_LAT_MAX-TIMEZONE_LAT_MIN)
#define TIMEZONE_LON_RANGE ((float)TIMEZONE_LON_MAX-TIMEZONE_LON_MIN)

/* structure --------------------------------------------------------------- */
/* class ------------------------------------------------------------------- */
class Timezone
{
	private:
	
	public:
		
		Timezone(void);
		uint8_t offset(float latitude, float longitude);
};

#endif
