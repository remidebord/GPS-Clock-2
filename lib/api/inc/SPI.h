#ifndef __SPI_H
#define __SPI_H

/* includes ---------------------------------------------------------------- */
#include "GPIO.h"

/* class ------------------------------------------------------------------- */
class SPI
{
	private:
		
		SPI_TypeDef* m_spi;
	
		GPIO m_cs;
		GPIO m_sck;
		GPIO m_mosi; // sdi
		GPIO m_miso; // sdo
	
	public:
		
		SPI(SPI_TypeDef* spi, PinName cs, PinName sck, PinName mosi, PinName miso);

		void cs(uint8_t value);
		uint8_t write(uint8_t value);
		uint8_t read(void);
	
};

#endif
