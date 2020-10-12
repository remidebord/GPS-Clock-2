/*!
 * \file Analog.cpp
 * \brief Analog API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 05 janvier 2016
 *
 * Analog library (ADC and DAC).
 *
 */

#include "Analog.h"

__IO uint8_t AnalogIn::m_channels = 0;
__IO uint8_t AnalogIn::m_rank[ADC_CHANNELS_MAX] = {0};
__IO uint16_t AnalogIn::m_value[ADC_CHANNELS_MAX] = {0};

AnalogIn :: AnalogIn(PinName pin) : GPIO(pin, Pin_AN)
{
	uint8_t i = 0;
	
	this->pull(Pull_None);
	
	if((ADC1->CR & ADC_CR_ADSTART) != 0)
	{
		// Stop conversion
		ADC1->CR |= ADC_CR_ADSTP;
		
		// Wait ADC
		while((ADC1->CR & ADC_CR_ADSTP) != 0);
	}
	
	if((ADC1->CR & ADC_CR_ADEN) != 0)
	{		
		// Disable ADC
		ADC1->CR |= ADC_CR_ADDIS;
		
		// Wait ADC
		while((ADC1->CR & ADC_CR_ADDIS) != 0);
	}
	
	// ADC configuration needed ?
	if(ADC1->CFGR1 == 0)
	{
		// DMA access: disable
		ADC1->CFGR1 &= (uint32_t)(~ADC_CFGR1_DMAEN);
	
		AnalogIn::adc();
		
		// Initialize rank array
		for(i = 0; i < ADC_CHANNELS_MAX; i++)
		{
			m_rank[i] = 0xFF;
		}
	}
	
	// Get ADC channel
	m_channel = AnalogIn::channel(pin);
	
	// Store channel index
	m_rank[m_channel] = m_channels++;
	
	// Sort rank of channel already configured
	AnalogIn::sort((uint8_t*)&m_rank[0], ADC_CHANNELS_MAX);
	
	// DMA configuration
	AnalogIn::dma();	
	
	// Channel configuration
	ADC1->CHSELR |= ((uint32_t)0x01 << m_channel);
	
	// Enable ADC
	ADC1->CR |= ADC_CR_ADEN;
	
	// Wait ADC
	while((ADC1->ISR & ADC_ISR_ADRDY) == 0);
	
	// Start conversion
	ADC1->CR |= ADC_CR_ADSTART;
}

void AnalogIn :: adc(void)
{	
	// Enable ADC1 clock
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	
	// ADC clock mode: PCLK/2 = 48MHz/2
	ADC1->CFGR2 = ADC_CFGR2_JITOFFDIV2;
	
	// Sampling time configuration (239.5 ADC clock cycles)
	ADC1->SMPR |= ADC_SMPR1_SMPR;
	
	// Conversion time = (239.5 + 12.5) x (1 / 24MHz) = 10us
	
	// Clear configuration register 1
	ADC1->CFGR1 = 0;
	
	// Default:
	// - Conversion mode: single
	// - Overrun: use old data
	// - Data alignment: right
	// - Data resolution: 12 bits
	// - Scan direction: upward
	// - DMA configuration: one shot mode
	// - DMA access : disabled
	
	// Overrun: overwrite
	ADC1->CFGR1 |= ADC_CFGR1_OVRMOD;
	
	// Conversion mode: continuous
	ADC1->CFGR1 |= ADC_CFGR1_CONT;
	
	// DMA configuration: circular mode
	ADC1->CFGR1 |= ADC_CFGR1_DMACFG;
	
	// DMA access: enable
	ADC1->CFGR1 |= ADC_CFGR1_DMAEN;
}

void AnalogIn :: dma(void)
{
	// Disable DMA1 channel1
	DMA1_Channel1->CCR &= (uint16_t)(~DMA_CCR_EN);
	
	// Enable DMA clock
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	
	// Clear DMA1 channel1 configuration register
	DMA1_Channel1->CCR = 0;
	
	// Channel priority: high
	DMA1_Channel1->CCR |= DMA_CCR_PL_1;
	
	// Memory size: 16 bits
	DMA1_Channel1->CCR |= DMA_CCR_MSIZE_0;
	
	// Peripheral size: 16 bits
	DMA1_Channel1->CCR |= DMA_CCR_PSIZE_0;
	
	// Memory increment method: enabled
	DMA1_Channel1->CCR |= DMA_CCR_MINC;
	
	// Circular mode: enabled
	DMA1_Channel1->CCR |= DMA_CCR_CIRC;
	
	// Data transfer direction: peripheral to memory
	
	// Number of data to transfer
	DMA1_Channel1->CNDTR = (uint32_t)m_channels;
	
	// Peripheral data register address
	DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;
	
	// Memory address to which the data will be written
	DMA1_Channel1->CMAR = (uint32_t)&m_value[0];
	
	// Enable DMA1 channel1
	DMA1_Channel1->CCR |= DMA_CCR_EN;
}

uint8_t AnalogIn :: channel(PinName pin)
{
	uint32_t port = pin & 0xFFFFFF00;
	uint8_t in = (uint8_t)pin;
	uint8_t channel = 0;
	
	switch(port)
	{
		case GPIOA_BASE: channel = (in + ADC_CHANNEL_GPIOA_OFFSET) ; break;
		case GPIOB_BASE: channel = (in + ADC_CHANNEL_GPIOB_OFFSET); break;
		case GPIOC_BASE: channel = (in + ADC_CHANNEL_GPIOC_OFFSET); break;
		default: break;
	}
	
	return channel;
}

void AnalogIn :: sort(uint8_t* buffer, uint8_t size)
{
	uint8_t i = 0;
	uint8_t j = 0;
	
	for(i = 0; i < size; i++)
	{
		if(buffer[i] != 0xFF)
		{
			buffer[i] = j++; 
		}
	}
}	

uint16_t AnalogIn :: read_b(void)
{
	// End of sequence ?
	while((ADC1->ISR & ADC_ISR_EOSEQ) == 0); 
    
	// Clear flags
	ADC1->ISR |= ADC_ISR_EOSEQ;
    
	return m_value[m_rank[m_channel]];
}

uint16_t AnalogIn :: read(void)
{
	return m_value[m_rank[m_channel]];
}

AnalogIn :: operator uint16_t()
{
	return m_value[m_rank[m_channel]];
}

/////////////////////

AnalogOut :: AnalogOut(PinName pin) : GPIO(pin, Pin_AN)
{
	this->pull(Pull_None);

	// Enable DAC clock
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	
	// Enable DAC channel 1
	DAC->CR |= DAC_CR_EN1;
}

void AnalogOut :: write(uint16_t value)
{
	DAC->DHR12R1 = value;;
}

AnalogOut& AnalogOut ::  operator= (uint16_t value)
{
	DAC->DHR12R1 = value;
	return *this;
}

uint16_t AnalogOut :: read()
{
	return DAC->DOR1;
}

AnalogOut :: operator uint16_t()
{
	return DAC->DOR1;
}

