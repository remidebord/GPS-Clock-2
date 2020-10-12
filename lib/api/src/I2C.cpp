/*!
 * \file I2C.cpp
 * \brief I2C API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 23 janvier 2017
 *
 * I2C library (400kHz).
 *
 */

#include "I2C.h"

extern "C"
{
	static CircularBuffer* bufferRx[2];
	static CircularBuffer* bufferTx[2];
}

I2C :: I2C(I2C_TypeDef* i2c, PinName sda, PinName scl): m_sda(sda, Pin_AF), m_scl(scl, Pin_AF),
                                                        m_circularRx(&m_bufferRx[0], I2C_BUFFER_SIZE),
                                                        m_circularTx(&m_bufferTx[0], I2C_BUFFER_SIZE)
{
	uint32_t tmp = 0;
	uint8_t index = 0;
	
	IRQn_Type irq = I2C1_IRQn;
	
	m_i2c = i2c;
  
	// Enable I2C clock
	switch((uint32_t)i2c)
	{
		case I2C1_BASE: RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; break;
		case I2C2_BASE: RCC->APB1ENR |= RCC_APB1ENR_I2C2EN; break;
		default: break;
	}
	
	// Configure pins
	m_sda.type(Open_Drain);
	m_scl.type(Open_Drain);
	
	m_sda.pull(Pull_None);
	m_scl.pull(Pull_None);
	
	m_sda.af(AF_1);
	m_scl.af(AF_1);
	
	// Link circular buffer
	if(i2c == I2C1) index = 0;
	else if(i2c == I2C2) index = 1;
	
	bufferRx[index] = &m_circularRx;
	bufferTx[index] = &m_circularTx;
	
	// I2C clock: PCLK (APB) 48MHz (I2C2 use SYSCLK = PCLK, by default)
	if(i2c == I2C1) RCC->CFGR3 |= RCC_CFGR3_I2C1SW;
	
	// Reset I2C peripheral
	if(i2c == I2C1) tmp = RCC_APB1ENR_I2C1EN;
	else if(i2c == I2C2) tmp = RCC_APB1ENR_I2C2EN;
	
	RCC->APB1RSTR |= tmp;
	RCC->APB1RSTR &= ~tmp;
	
	// I2C configuration (I2C Fast mode 400kHz, see table 81)
	// PRESC: 0x5, SCLDEL: 0x3, SDADEL: 0x3, SCLH: 0x3, SCLL:0x9
	//m_i2c->TIMINGR = 0x50330309;
	m_i2c->TIMINGR = 0xB0420F13;

	// I2C interrupt
	m_i2c->CR1 |= I2C_CR1_TXIE;
	m_i2c->CR1 |= I2C_CR1_RXIE;
	m_i2c->CR1 |= I2C_CR1_TCIE;   // Transfer complete
	m_i2c->CR1 |= I2C_CR1_STOPIE;	// STOP detection
	m_i2c->CR1 |= I2C_CR1_NACKIE;	// Not acknowledge received
	m_i2c->CR1 |= I2C_CR1_ERRIE;

	// NVIC configuration
	if(i2c == I2C1) irq = I2C1_IRQn;
	else if(i2c == I2C2) irq = I2C2_IRQn;
	
	NVIC_SetPriority(irq, 1); // High: 0, Low: 3
	NVIC_EnableIRQ(irq);

	// Enable I2C
	m_i2c->CR1 |= I2C_CR1_PE;
}

uint8_t I2C :: read(uint8_t address, uint8_t* buffer, uint8_t length)
{
	uint8_t result = 0;
	uint8_t i = 0;
	
	if(this->busy() == 0)
	{
		if(length == m_circularRx.count())
		{
			for(i = 0; i < length; i++)
			{
				buffer[i] = m_circularRx.get();
			}
			
			m_i2c->CR2 = 0x00;
			
			result = 1;
		}
		else if(m_i2c->CR2 == 0x00)
		{
			// Slave address (7 bit)
			m_i2c->CR2 |= address;
			
			// Number of bytes to transfer
			m_i2c->CR2 |= (length << 16);
			
			// Transfer direction: read
			m_i2c->CR2 |= I2C_CR2_RD_WRN;
			
			// START generation
			m_i2c->CR2 |= I2C_CR2_START;
		}
	}
	
	return result;
}

uint8_t I2C :: write(uint8_t address, uint8_t* buffer, uint8_t length)
{
	uint8_t result = 0;
	uint8_t i = 0;
	
	if(this->busy() == 0)
	{
		if(m_circularTx.count() == 0)
		{
			for(i = 0; i < length; i++)
			{
				m_circularTx.put(buffer[i]);
			}
			
			// Slave address (7 bit)
			m_i2c->CR2 |= address;
			
			// Number of bytes to transfer
			m_i2c->CR2 |= (length << 16);
			
			// Transfer direction: write
			m_i2c->CR2 &= ~I2C_CR2_RD_WRN;
			
			// START generation
			m_i2c->CR2 |= I2C_CR2_START;
			
			result = 1;
		}
	}
	
	return result;
}

void I2C :: read_b(uint8_t address, uint8_t* buffer, uint8_t length)
{
	while(this->read(address, buffer, length) == 0); 
}

void I2C :: write_b(uint8_t address, uint8_t* buffer, uint8_t length)
{
	while(this->write(address, buffer, length) == 0); 
}

uint8_t I2C :: busy(void)
{
	uint8_t result = 0;
	
	if((I2C1->ISR & I2C_ISR_BUSY) != 0)
	{
		result = 1;
	}
	
	return result;
}

extern "C"
{	
	void I2C1_IRQHandler(void)
	{
		uint32_t tmp = 0;
		
		// Transmit complete
		if((I2C1->ISR & I2C_ISR_TC) != 0)
		{
			// STOP generation
			I2C1->CR2 |= I2C_CR2_STOP;	
		}
		
		// Transmit buffer interrupt status
		if((I2C1->ISR & I2C_ISR_TXIS) != 0)
		{
			// Data to send ?
			if(bufferTx[0]->count() && (bufferTx[0] != 0))
			{
				I2C1->TXDR = bufferTx[0]->get();
			}
		}
		
		// Receive buffer interrupt status
		if((I2C1->ISR & I2C_ISR_RXNE) != 0)
		{
			if(bufferRx[0] != 0)
			{
				bufferRx[0]->put(I2C1->RXDR);
			}
		}
		
		// STOP generated
		if((I2C1->ISR & I2C_ISR_STOPF) != 0)
		{
			I2C1->ICR |= I2C_ICR_STOPCF;
			I2C1->CR2 = 0;
		}
		
		// Errors
		if((I2C1->ISR & I2C_ISR_NACKF) != 0) tmp |= I2C_ICR_NACKCF;
		if((I2C1->ISR & I2C_ISR_BERR) != 0) tmp |= I2C_ISR_BERR;
		if((I2C1->ISR & I2C_ISR_ARLO) != 0) tmp |= I2C_ISR_ARLO;
		if((I2C1->ISR & I2C_ISR_OVR) != 0) tmp |= I2C_ISR_OVR;
		if((I2C1->ISR & I2C_ISR_PECERR) != 0) tmp |= I2C_ISR_PECERR;
		if((I2C1->ISR & I2C_ISR_TIMEOUT) != 0) tmp |= I2C_ISR_TIMEOUT;
		if((I2C1->ISR & I2C_ISR_ALERT) != 0) tmp |= I2C_ISR_ALERT;
		
		if(tmp != 0)
		{
			I2C1->ICR |= tmp;
			
			if(bufferRx[0] != 0)
			{
				bufferRx[0]->flush();
			}
			
			I2C1->CR2 = 0;			
		}
	}
	
	void I2C2_IRQHandler(void)
	{
		uint32_t tmp = 0;
		
		// Transmit complete
		if((I2C2->ISR & I2C_ISR_TC) != 0)
		{
			// STOP generation
			I2C2->CR2 |= I2C_CR2_STOP;	
		}
		
		// Transmit buffer interrupt status
		if((I2C2->ISR & I2C_ISR_TXIS) != 0)
		{
			// Data to send ?
			if(bufferTx[1]->count() && (bufferTx[1] != 0))
			{
				I2C2->TXDR = bufferTx[1]->get();
			}
		}
		
		// Receive buffer interrupt status
		if((I2C2->ISR & I2C_ISR_RXNE) != 0)
		{
			if(bufferRx[1] != 0)
			{
				bufferRx[1]->put(I2C2->RXDR);
			}
		}
		
		// STOP generated
		if((I2C2->ISR & I2C_ISR_STOPF) != 0)
		{			
			I2C2->ICR |= I2C_ICR_STOPCF;
			I2C2->CR2 = 0;
		}
		
		// Errors
		if((I2C2->ISR & I2C_ISR_NACKF) != 0) tmp |= I2C_ICR_NACKCF;
		if((I2C2->ISR & I2C_ISR_BERR) != 0) tmp |= I2C_ISR_BERR;
		if((I2C2->ISR & I2C_ISR_ARLO) != 0) tmp |= I2C_ISR_ARLO;
		if((I2C2->ISR & I2C_ISR_OVR) != 0) tmp |= I2C_ISR_OVR;
		if((I2C2->ISR & I2C_ISR_PECERR) != 0) tmp |= I2C_ISR_PECERR;
		if((I2C2->ISR & I2C_ISR_TIMEOUT) != 0) tmp |= I2C_ISR_TIMEOUT;
		if((I2C2->ISR & I2C_ISR_ALERT) != 0) tmp |= I2C_ISR_ALERT;
		
		if(tmp != 0)
		{
			I2C2->ICR |= tmp;
			
			if(bufferRx[1] != 0)
			{
				bufferRx[1]->flush();
			}
			
			I2C2->CR2 = 0;			
		}
	}
}
