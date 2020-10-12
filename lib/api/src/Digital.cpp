/*!
 * \file Digital.cpp
 * \brief Digital API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 31 décembre 2016
 *
 * Digital library (DigitalIn, DigitalOut and InterruptIn).
 *
 */

#include "Digital.h"

extern "C"
{
	void (*extiCallback[16])(void);
}

DigitalOut :: DigitalOut(PinName pin) : GPIO(pin, Pin_Output)
{
	this->type(Push_Pull);
	this->pull(Pull_None);
	
	this->write(0);
}

DigitalOut& DigitalOut :: operator= (uint32_t value)
{
	this->write(value);
	return *this;
}

/////////////////////

DigitalIn :: DigitalIn(PinName pin) : GPIO(pin, Pin_Input)
{
	this->pull(Pull_Down);
}

/////////////////////

InterruptIn :: InterruptIn(PinName pin) : GPIO(pin, Pin_Input)
{
	uint32_t port = 0;
	uint8_t shift = 0;
	uint8_t index = 0;
	
	IRQn_Type irq = EXTI0_1_IRQn;
	
	this->pull(Pull_None);
	
	// Enable SYSCFG clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	port  = ((((uint32_t)m_port) - AHB2PERIPH_BASE) >> 10);
	shift = (0x04 * (m_pin & 0x03));
	index = m_pin >> 0x02;
	
	// SYSCFG external interrupt configuration
	SYSCFG->EXTICR[index] &= ~(0x0F << shift);
	SYSCFG->EXTICR[index] |= (port << shift);
	
	// Clear EXTI line configuration
	EXTI->IMR &= ~m_mask;
	EXTI->EMR &= ~m_mask;
	
	// Select interrupt mode
	EXTI->IMR |= m_mask;
	
	// Clear Rising Falling edge configuration
	EXTI->RTSR &= ~m_mask;
	EXTI->FTSR &= ~m_mask;
	
	// NVIC configuration
	if(m_pin <= 1) irq = EXTI0_1_IRQn;
	else if (m_pin <= 3) irq = EXTI2_3_IRQn;
	else irq = EXTI4_15_IRQn;
	
	NVIC_SetPriority(irq, 1); // High: 0, Low: 3
	NVIC_EnableIRQ(irq);
}

void InterruptIn :: rise(void(*f)(void))
{
	extiCallback[m_pin] = f;
	
	EXTI->RTSR |= m_mask;
}

void InterruptIn :: fall(void(*f)(void))
{	
	extiCallback[m_pin] = f;
	
	EXTI->FTSR |= m_mask;
}

void InterruptIn :: risefall(void(*f)(void))
{	
	extiCallback[m_pin] = f;
	
	EXTI->RTSR |= m_mask;
	EXTI->FTSR |= m_mask;
}

extern "C"
{
	void EXTI0_1_IRQHandler(void)
	{
		uint32_t extiLine = 0;
		uint8_t i = 0;
		
		for(i = 0; i <= 1; i++)
		{
			extiLine = ((uint32_t)0x01 << i);
			
			// Check pending register
			if ((EXTI->PR & (extiLine)) != 0)
			{
				// Callback ?
				if(extiCallback[i] != 0) (*extiCallback[i])();
				
				// Clear bit
				EXTI->PR = extiLine;
			}			
		}
	}
	
	void EXTI2_3_IRQHandler(void)
	{
		uint32_t extiLine = 0;
		uint8_t i = 0;
		
		for(i = 2; i <= 3; i++)
		{
			extiLine = ((uint32_t)0x01 << i);
			
			// Check pending register
			if ((EXTI->PR & (extiLine)) != 0)
			{				
				// Callback ?
				if(extiCallback[i] != 0) (*extiCallback[i])();
				
				// Clear bit
				EXTI->PR = extiLine;
			}			
		}
	}
	
	void EXTI4_15_IRQHandler(void)
	{
		uint32_t extiLine = 0;
		uint8_t i = 0;
		
		for(i = 4; i <= 15; i++)
		{
			extiLine = ((uint32_t)0x01 << i);
			
			// Check pending register
			if ((EXTI->PR & (extiLine)) != 0)
			{				
				// Callback ?
				if(extiCallback[i] != 0) (*extiCallback[i])();
				
				// Clear bit
				EXTI->PR = extiLine;
			}			
		}
	}
}

