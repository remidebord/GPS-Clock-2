/*!
 * \file DS3231.cpp
 * \brief DS3231 I2C RTC API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 28 janvier 2017
 *
 * DS3231 High Precision RTC library.
 *
 */

#include "DS3231.h"

DS3231 :: DS3231(I2C* i2cx, InterruptIn* squareWave)
{
	uint8_t* ptr = 0;
	uint8_t i = 0;
	
	m_i2c = i2cx;
	m_squareWave = squareWave;
	m_timer = DS3231_STARTUP_TIME;
	m_state = DS3231_INIT;
	m_initState = DS3231_INIT_RESET_REGISTER_POINTER;
	m_initNextState = DS3231_INIT_READ_REGISTERS;
	
	ptr = (uint8_t*)&m_ds3231;
	
	for(i = 0; i < sizeof(m_ds3231); i++)
	{
		ptr[i] = 0x00;
	}
	
	m_sync = 1;
	m_write = 0;
}

void DS3231 :: update(void)
{
	uint8_t result = 0;
	uint8_t tmp = 0;
	
	if(m_timer == 0)
	{
		switch(m_state)
		{
			case DS3231_INIT:
				// Sub state machine for initialization
				switch(m_initState)
				{
					case DS3231_INIT_RESET_REGISTER_POINTER:
						// Reset the register pointer by writing 0x00 (start address)
						result = m_i2c->write(DS3231_I2C_ADDR, (uint8_t*)&tmp, sizeof(tmp));
					
						// Write OK ?
						if(result != 0)
						{
							m_initState = m_initNextState;
							m_timer = DS3231_SILENCE_TIME;
						}
						break;
					
					case DS3231_INIT_READ_REGISTERS:
						// Read all registers
						result = m_i2c->read(DS3231_I2C_ADDR, (uint8_t*)&(m_ds3231.SECR), sizeof(m_ds3231)-1);
					
						if(result != 0)
						{
							m_initState = DS3231_INIT_RESET_REGISTER_POINTER;
							m_initNextState = DS3231_INIT_WRITE_REGISTERS;
							m_timer = DS3231_SILENCE_TIME;
						}
						break;
					
					case DS3231_INIT_WRITE_REGISTERS:
						// Enable square wave
						m_ds3231.CR &= (~(0x1C)); // 1Hz
	
						// Write all registers
						result = m_i2c->write(DS3231_I2C_ADDR, (uint8_t*)&m_ds3231, sizeof(m_ds3231));
					
						if(result != 0)
						{
							m_initState = DS3231_INIT_RESET_REGISTER_POINTER;
							m_initNextState = DS3231_INIT_READ_REGISTERS;
							m_state = DS3231_RESET_REGISTER_POINTER;
							m_timer = DS3231_SILENCE_TIME;
						}
						break;
					
					default: break;
				}
				break;
			
			case DS3231_IDLE:
				// Write request ?
				if(m_write != 0)
				{
					m_write = 0;
					m_state = DS3231_WRITE_REGISTERS;
				}
				else
				{
					// Periodic refresh of the DS3231 registers in RAM
					m_state = DS3231_READ_REGISTERS;
				}
				
				m_timer = DS3231_SILENCE_TIME;
				break;
			
			case DS3231_READ_REGISTERS:
				// No write operation pending ?
				if(m_write == 0)
				{
					// Read all registers
					result = m_i2c->read(DS3231_I2C_ADDR, (uint8_t*)&(m_ds3231.SECR), sizeof(m_ds3231)-1);
				}
				else result = 1;
				
				// Read OK ?
				if(result != 0)
				{
					// Synchronize time and date ?
					if((m_sync != 0) && (m_write == 0))
					{
						m_sync = 0;
						
						// Synchronise time and date
						m_time.seconds = bcd2bin(m_ds3231.SECR & 0x7F);
						m_time.minutes = bcd2bin(m_ds3231.MINR & 0x7F);
						m_time.hours = bcd2bin(m_ds3231.HOUR & 0x3F);
						
						m_date.day = bcd2bin(m_ds3231.DATR & 0x3F);
						m_date.month = bcd2bin(m_ds3231.MONR & 0x1F);
						m_date.year = bcd2bin(m_ds3231.YEAR);
					}
						
					m_state = DS3231_RESET_REGISTER_POINTER;
					m_timer = DS3231_SILENCE_TIME;
				}
				break;
			
			case DS3231_WRITE_REGISTERS:
				// Write all registers
				result = m_i2c->write(DS3231_I2C_ADDR, (uint8_t*)&m_ds3231, sizeof(m_ds3231));
			
				// Write OK ?
				if(result != 0)
				{
					m_state = DS3231_RESET_REGISTER_POINTER;
					m_timer = DS3231_SILENCE_TIME;
				}
				break;
			
			case DS3231_RESET_REGISTER_POINTER:
				// Reset the register pointer by writing 0x00 (start address)
				result = m_i2c->write(DS3231_I2C_ADDR, (uint8_t*)&tmp, sizeof(tmp));
			
				// Write OK ?
				if(result != 0)
				{
					m_state = DS3231_IDLE;
				}
				break;
			
			default: break;
		}
	}
}

void DS3231 :: set(Time* time)
{
	m_ds3231.SECR = bin2bcd(time->seconds);
	m_ds3231.MINR = bin2bcd(time->minutes);
	m_ds3231.HOUR = bin2bcd(time->hours);
	
	m_time.seconds = time->seconds;
	m_time.minutes = time->minutes;
	m_time.hours = time->hours;
	
	m_write = 1;
	m_sync = 1;
}

void DS3231 :: set(Date* date)
{
	m_ds3231.DATR = bin2bcd(date->day);
	m_ds3231.MONR = bin2bcd(date->month);
	m_ds3231.YEAR = bin2bcd(date->year);
	
	m_date.day = date->day;
	m_date.month = date->month;
	m_date.year = date->year;
	
	m_write = 1;
}

void DS3231 :: square_wave(uint8_t state)
{
	// Enable ?
	if(state != 0)
	{
		m_ds3231.CR &= (~(0x1C)); // 1Hz
	}
	else
	{
		m_ds3231.CR |= 0x1C;
	}
	
	m_write = 1;
}

void DS3231 :: get(Time *time)
{
	time->seconds = m_time.seconds;
	time->minutes = m_time.minutes;
	time->hours = m_time.hours;
}

void DS3231 :: get(Date *date)
{
	date->day = m_date.day;
	date->month = m_date.month;
	date->year = m_date.year;
}

uint8_t DS3231 :: temperature(void)
{
	return m_ds3231.HTEMP;
}

//float DS3231 :: temperature(void)
//{
//	uint16_t tmp = 0;
//	
//	tmp = (m_ds3231.HTEMP << 8) + m_ds3231.LTEMP;
//	
//	// Convert fixed point to float (Qm.n with m = 7 and n = 8)
//	// f = fp / 2^n
//	return ((float)tmp / 256);
//}

void DS3231 :: sync(void)
{
	m_sync = 1;
}

void DS3231 :: irq(void)
{
	// Falling edge ?
	if(m_squareWave->read() == 0)
	{
		m_time.seconds++;
			
		if(m_time.seconds >= 60)
		{
			m_time.seconds = 0;
			m_time.minutes++;
		}
			
		if(m_time.minutes >= 60)
		{
			m_time.minutes = 0;
			m_time.hours++;
		}
			
		if(m_time.hours >= 24)
		{
			m_time.seconds = 0;
			m_time.minutes = 0;
			m_time.hours = 0;
		}
	}
}

void DS3231 :: timer(void)
{
	if(m_timer) m_timer--;
}

