#include "GPS.h"

GNSS :: GNSS(Serial* serial)
{
	m_serial = serial;
}

void GNSS :: update(void)
{
	uint8_t buffer[USART_BUFFER_SIZE] = {0};
	uint16_t length = 0;
	uint16_t i = 0;

	uint32_t sentence = 0;
	uint16_t termLength = 0;
	uint16_t termIndex = 0;
	
	length = m_serial->read(buffer);
	
	// Data received ?
	if(length > 0)
	{
		length++;
		
		for(i = 0; i < length; i++)
		{
			switch(buffer[i])
			{
				case '$': // Sentence begin
					sentence = 0;
					termLength = 0;
					break;
				
				case ',': // Term end
					if(sentence == 0)
					{						
						// GGA, RMC, ...
						sentence += (buffer[i-3] << 24);
						sentence += (buffer[i-2] << 16);
						sentence += (buffer[i-1] << 8);
					}
					else this->parse(&sentence, &buffer[termIndex], termLength);
					
					termIndex = i + 1;
					termLength = 0;
					break;
				
				case '*': // Checksum 
					break;
				
				case '\r': // Sentence end
				case '\n':
					sentence = 0;
					break; 
				
				default: // Characters
					termLength++;
					break;
			}
		}
	}
}

void GNSS :: parse(uint32_t* sentence, uint8_t* buffer, uint16_t length)
{
	float degrees = 0;
	float minutes = 0;
	float speed = 0;
	uint8_t i = 0;
	
	switch(*sentence)
	{
		case GGA_Time:
		case RMC_Time:
			if(length >= 6)
			{
				m_time.hours   = ((buffer[0] - '0') * 10) + (buffer[1] - '0');
				m_time.minutes = ((buffer[2] - '0') * 10) + (buffer[3] - '0');
				m_time.seconds = ((buffer[4] - '0') * 10) + (buffer[5] - '0');
			}
			break;
		
		case GGA_Latitude_1:
		case RMC_Latitude_1:
			if((length >= 8) && (buffer[4] == '.'))
			{
				degrees  = ((buffer[0] - '0') * 10) + (buffer[1] - '0');
				
				minutes  = ((buffer[2] - '0') * 10) + (buffer[3] - '0');
				minutes += ((buffer[5] - '0') * 0.1) + ((buffer[6] - '0') * 0.01);
				
				m_latitude = degrees + (minutes / 60); 
			}
			break;
			
		case GGA_Latitude_2:
		case RMC_Latitude_2:
			if(length == 1)
			{
				if(buffer[0] == 'S')
				{
					m_latitude = -m_latitude;
				}
			}
			break;
		
		case GGA_Longitude_1:
		case RMC_Longitude_1:
			if((length >= 9) && (buffer[5] == '.'))
			{
				degrees  = ((buffer[0] - '0') * 100) + ((buffer[1] - '0') * 10) + (buffer[2] - '0');
				
				minutes  = ((buffer[3] - '0') * 10) + (buffer[4] - '0');
				minutes += ((buffer[6] - '0') * 0.1) + ((buffer[7] - '0') * 0.01);
				
				m_longitude = degrees + (minutes / 60);  
			}
			break;
		
		case GGA_Longitude_2:
		case RMC_Longitude_2:
			if(length == 1)
			{
				if(buffer[0] == 'W')
				{
					m_longitude = -m_longitude;
				}
			}
			break;
		
		case GGA_Fix:
			if(length == 1)
			{
				m_fix = buffer[0] - '0';
			}
			break;
		
		case GGA_Satellites:
			if(length == 2)
			{
				m_satellites  = (buffer[0] - '0') * 10;
				m_satellites += (buffer[1] - '0');
			}
			break;
			
		case GGA_Altitude:
			if(length > 0)
			{
				m_altitude = 0;
				
				for(i = 0; i < length; i++)
				{
					if(buffer[i] != '.')
					{
						m_altitude = (m_altitude * 10) + (buffer[i] - '0');
					}
					else i = length;
				}
			}
			break;
			
		case RMC_Speed:
			if((length >= 9) && (buffer[5] == '.'))
			{
				speed  = ((buffer[0] - '0') * 100) + ((buffer[1] - '0') * 10) + (buffer[2] - '0');
				speed += ((buffer[2] - '0') * 0.1);

				// Convert in km/h
				m_speed = speed * GNSS_KNOT;  
			}
			break;
			
		case RMC_Date:
			if(length >= 6)
			{
				m_date.day   = ((buffer[0] - '0') * 10) + (buffer[1] - '0');
				m_date.month = ((buffer[2] - '0') * 10) + (buffer[3] - '0');
				m_date.year  = ((buffer[4] - '0') * 10) + (buffer[5] - '0');
			}
			break;
		
		default: break;
	}
	
	*sentence = *sentence + 1;
}

void GNSS :: time(Time *t)
{
	t->hours = m_time.hours;
	t->minutes = m_time.minutes;
	t->seconds = m_time.seconds;
}

void GNSS :: date(Date *d)
{
	d->day = m_date.day;
	d->month = m_date.month;
	d->year = m_date.year;
}

void GNSS :: position(float* latitude, float* longitude)
{
	*latitude = m_latitude;
	*longitude = m_longitude;
}

uint16_t GNSS :: altitude(void)
{
	return m_altitude;
}

uint8_t GNSS :: fix(void)
{
	return m_fix;
}

uint8_t GNSS :: satellites(void)
{
	return m_satellites;
}

float GNSS :: speed(void)
{
	return m_speed;
}
