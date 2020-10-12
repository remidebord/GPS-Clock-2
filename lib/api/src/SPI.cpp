/*!
 * \file SPI.cpp
 * \brief Serial Peripheral Interface API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 24 janvier 2016
 *
 * SPI library.
 *
 */

#include "SPI.h"

SPI :: SPI(SPI_TypeDef* spi, PinName cs, PinName sck, PinName mosi, PinName miso): m_cs(cs, Pin_Output), m_sck(sck, Pin_AF), 
                                                                                   m_mosi(mosi, Pin_AF), m_miso(miso, Pin_AF)
{
	m_spi = spi;
  
	// Enable SPI clock
	switch((uint32_t)spi)
	{
		case SPI1_BASE: RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; break;
		case SPI2_BASE: RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; break;
		default: break;
	}
	
	// Configure pins
	m_sck.type(Push_Pull);
	m_miso.type(Push_Pull);
	m_mosi.type(Push_Pull);
	m_cs.type(Push_Pull);
	
	m_sck.pull(Pull_Up);
	m_mosi.pull(Pull_Up);
	m_miso.pull(Pull_Up);
	m_cs.pull(Pull_Up);
	
	m_sck.af(AF_0);
	m_mosi.af(AF_0);
	m_miso.af(AF_0);
  
	// SPI configuration (Master, Full duplex, 6MHz)
	m_spi->CR1 = 0;
	
	// Baudrate (48MHz / 8 = 6MHz)
	m_spi->CR1 |= SPI_CR1_BR_1;
	
	// NSS (CS) software
	m_spi->CR1 |= SPI_CR1_SSM;
	
	// Master
	m_spi->CR1 |= (SPI_CR1_MSTR | SPI_CR1_SSI);
	
	// CPOL: 1 when idle, CPHA: 2 edges
	m_spi->CR1 |= (SPI_CR1_CPOL | SPI_CR1_CPHA);
	
	// Data size (8 bits)
	m_spi->CR2 |= (SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2);
	
	// RXNE event generated every 8 bits (FIFO/4)
	m_spi->CR2 |= SPI_CR2_FRXTH;
	
	// Enable SPI
	m_spi->CR1 |= SPI_CR1_SPE;
}

void SPI :: cs(uint8_t value)
{
	m_cs.write(value);
}

uint8_t SPI :: write(uint8_t value)
{
	while((m_spi->SR & SPI_SR_TXE) == 0);
		
	*(__IO uint8_t *)&m_spi->DR = value;
	
	while((m_spi->SR & SPI_SR_RXNE) == 0);
	
	return (uint8_t)m_spi->DR;
}

uint8_t SPI :: read(void)
{
	*(__IO uint8_t *)&m_spi->DR = 0xFF; // Dummy
	
	while((m_spi->SR & SPI_SR_RXNE) == 0);
	
	return (uint8_t)m_spi->DR;
}
