#ifndef __SERIAL_H
#define __SERIAL_H

/* includes ----------------------------------------------------------------- */
#include "GPIO.h"
#include "CircularBuffer.h"

/* defines ------------------------------------------------------------------ */
#define USART_BAUDRATE_DEFAULT   (9600)
#define USART_BUFFER_SIZE        (512) // !important: shall be a power of 2 (32, 64, 128, 256, ...)

/* class -------------------------------------------------------------------- */
class Serial
{
	private:
		
		USART_TypeDef* m_usart;
		
		GPIO m_rx;
		GPIO m_tx;
		GPIO m_de;

		uint32_t m_baudrate;
		
		uint8_t m_bufferRx[USART_BUFFER_SIZE];
		uint8_t m_bufferTx[USART_BUFFER_SIZE];
	
		CircularBuffer m_circularRx;
		CircularBuffer m_circularTx;
	
		static void pin(GPIO* gpio);
	
	public:
		
		Serial(USART_TypeDef* usart, PinName rx, PinName tx, PinName de = NC);
		
		void baudrate(uint32_t value);
		void format(uint8_t databits, SerialParity parity, uint8_t stopbits);
		uint8_t write(uint8_t* buffer, uint16_t length);
		uint16_t read(uint8_t* buffer);
};

#endif /* __SERIAL_H */
