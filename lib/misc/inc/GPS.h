#ifndef __GPS_H
#define __GPS_H

/* includes ---------------------------------------------------------------- */
#include "Serial.h"
#include "RTC.h"

/* defines ----------------------------------------------------------------- */
#define GNSS_KNOT ((float)1.852) // km/h

#define	GNSS_GGA 0x474741
#define	GNSS_RMC 0x524D43

typedef enum{
	GGA_Time = (uint32_t)(GNSS_GGA << 8),
	GGA_Latitude_1,
	GGA_Latitude_2,
	GGA_Longitude_1,
	GGA_Longitude_2,
	GGA_Fix,
	GGA_Satellites,
	GGA_Dop,
	GGA_Altitude,
	RMC_Time = (uint32_t)(GNSS_RMC << 8),
	RMC_Status,
	RMC_Latitude_1,
	RMC_Latitude_2,
	RMC_Longitude_1,
	RMC_Longitude_2,
	RMC_Speed,
	RMC_Track,
	RMC_Date,
} GNSSTerm;

/* class ------------------------------------------------------------------- */
class GNSS
{
	private:
		
		Serial* m_serial;
	
		Time m_time;
		Date m_date;
	
		float m_latitude;
		float m_longitude;
		float m_speed;
	
		uint16_t m_altitude;
		uint8_t m_satellites;
		uint8_t m_fix;
	
	public:
		
		GNSS(Serial* serial);
	
		void update(void);
		void parse(uint32_t* sentence, uint8_t* buffer, uint16_t length);
	
		void time(Time *t);
		void date(Date *d);
		void position(float* latitude, float* longitude);
		uint16_t altitude(void);
		uint8_t fix(void);
		uint8_t satellites(void);
		float speed(void);
};

#endif
