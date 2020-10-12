#ifndef __DS3231_H
#define __DS3231_H

/* includes ---------------------------------------------------------------- */
#include "I2C.h"
#include "RTC.h"
#include "Digital.h"

/* defines ----------------------------------------------------------------- */
#define DS3231_STARTUP_TIME 10   // 10ms
#define DS3231_SILENCE_TIME 100  // 100ms

#define DS3231_I2C_ADDR 0xD0 // See DS3231 datasheet

typedef enum
{
	DS3231_INIT,
	DS3231_IDLE,
	DS3231_READ_REGISTERS,
	DS3231_WRITE_REGISTERS,
	DS3231_RESET_REGISTER_POINTER,
} T_DS3231_STATE;

typedef enum
{
	DS3231_INIT_RESET_REGISTER_POINTER,
	DS3231_INIT_READ_REGISTERS,
	DS3231_INIT_WRITE_REGISTERS,
} T_DS3231_INIT_STATE;

/* structures -------------------------------------------------------------- */
typedef struct
{
	uint8_t DUMMY;     /*!< !important (clear register pointer at write)           */
	uint8_t SECR;      /*!< DS3231 Seconds register,          Address offset: 0x00 */
	uint8_t MINR;      /*!< DS3231 Minutes register,          Address offset: 0x01 */
	uint8_t HOUR;      /*!< DS3231 Hours register,            Address offset: 0x02 */
	uint8_t DAYR;      /*!< DS3231 Day register,              Address offset: 0x03 */
	uint8_t DATR;      /*!< DS3231 Date register,             Address offset: 0x04 */
	uint8_t MONR;      /*!< DS3231 Month register,            Address offset: 0x05 */
	uint8_t YEAR;      /*!< DS3231 Year register,             Address offset: 0x06 */
	uint8_t ALRM1SECR; /*!< DS3231 Alarm 1 seconds register,  Address offset: 0x07 */
	uint8_t ALRM1MINR; /*!< DS3231 Alarm 1 minutes register,  Address offset: 0x08 */
	uint8_t ALRM1HOUR; /*!< DS3231 Alarm 1 hours register,    Address offset: 0x09 */
	uint8_t ALRM1DAR;  /*!< DS3231 Alarm 1 day/date register, Address offset: 0x0A */
	uint8_t ALRM2MINR; /*!< DS3231 Alarm 2 minutes register,  Address offset: 0x0B */
	uint8_t ALRM2HOUR; /*!< DS3231 Alarm 2 hours register,    Address offset: 0x0C */
	uint8_t ALRM2DAR;  /*!< DS3231 Alarm 2 day/date register, Address offset: 0x0D */
	uint8_t CR;        /*!< DS3231 Control register,          Address offset: 0x0E */
	uint8_t CSR;       /*!< DS3231 Control/Status register,   Address offset: 0x0F */
	uint8_t AOR;       /*!< DS3231 Aging offset register,     Address offset: 0x10 */
	uint8_t HTEMP;     /*!< DS3231 MSB of temp,               Address offset: 0x11 */
	uint8_t LTEMP;     /*!< DS3231 LSB of temp,               Address offset: 0x12 */
} DS3231_Typedef;

/* class ------------------------------------------------------------------- */
class DS3231
{
	private:
		
		I2C* m_i2c;
		InterruptIn* m_squareWave;
	
		Time m_time;
		Date m_date;
	
		uint16_t m_timer;
		T_DS3231_STATE m_state;
		T_DS3231_INIT_STATE m_initState;
		T_DS3231_INIT_STATE m_initNextState;
	
		uint8_t m_sync;
		uint8_t m_write;
	
		DS3231_Typedef m_ds3231;
	
	public:
		
		DS3231(I2C* i2cx, InterruptIn* squareWave);
		void update(void);
		void set(Time* time);
		void set(Date* date);
		void square_wave(uint8_t state);
		void get(Time* time);
		void get(Date* date);
		uint8_t temperature(void);
		//float temperature(void);
		void sync(void);
		void irq(void);
		void timer(void);
};

#endif
