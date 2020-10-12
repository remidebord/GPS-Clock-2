#ifndef __I2C_H
#define __I2C_H

/* includes ---------------------------------------------------------------- */
#include "GPIO.h"
#include "CircularBuffer.h"

/* defines ----------------------------------------------------------------- */
#define I2C_BUFFER_SIZE 32

/* class ------------------------------------------------------------------- */
class I2C
{
	private:
		
		I2C_TypeDef* m_i2c;
		GPIO m_sda;
		GPIO m_scl;
	
		uint8_t m_bufferRx[I2C_BUFFER_SIZE];
		uint8_t m_bufferTx[I2C_BUFFER_SIZE];
	
		CircularBuffer m_circularRx;
		CircularBuffer m_circularTx;
		
	public:
		
		I2C(I2C_TypeDef* i2c, PinName sda, PinName scl);
		uint8_t read(uint8_t address, uint8_t* buffer, uint8_t length);
		uint8_t write(uint8_t address, uint8_t* buffer, uint8_t length);
		uint8_t busy(void);
	
		void read_b(uint8_t address, uint8_t* buffer, uint8_t length);
		void write_b(uint8_t address, uint8_t* buffer, uint8_t length);
};

#endif
