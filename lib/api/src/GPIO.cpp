/*!
 * \file GPIO.cpp
 * \brief GPIO API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 31 décembre 2016
 *
 * GPIO library.
 *
 */

#include "GPIO.h"

GPIO :: GPIO(PinName pin, PinMode mode)
{	
	m_port = ((GPIO_TypeDef *)(pin & 0xFFFFFF00));
	m_pin = (uint8_t)pin;
	m_mask = ((uint32_t)0x01 << m_pin);
	
	if(pin != NC)
	{		
		// GPIO clock enable
		clock((uint32_t) m_port);
		
		// Configure pin in output
		this->mode(mode);
	}
}

void GPIO :: clock(uint32_t port)
{
	uint32_t tmp = 0;
	
	switch(port)
	{
		case GPIOA_BASE: tmp = RCC_AHBENR_GPIOAEN; break;
		case GPIOB_BASE: tmp = RCC_AHBENR_GPIOBEN; break;
		case GPIOC_BASE: tmp = RCC_AHBENR_GPIOCEN; break;
		case GPIOD_BASE: tmp = RCC_AHBENR_GPIODEN; break;
		case GPIOF_BASE: tmp = RCC_AHBENR_GPIOFEN; break;
		default: break;
	}
	
	RCC->AHBENR |= tmp;
}

void GPIO :: mode(PinMode mode)
{
	m_port->MODER  &= ~(GPIO_MODER_MODER0 << (m_pin * 2));
	m_port->MODER |= (((uint32_t)mode) << (m_pin * 2));
}

void GPIO :: type(PinType type)
{	
	m_port->OTYPER &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)m_pin));
	m_port->OTYPER |= (uint16_t)(type << ((uint16_t)m_pin));	
}

void GPIO :: pull(PinPull pull)
{
	m_port->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << ((uint16_t)m_pin * 2));
	m_port->PUPDR |= (((uint32_t)pull) << (m_pin * 2));	
}

void GPIO :: speed(PinSpeed speed)
{
	m_port->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 << (m_pin * 2));
	m_port->OSPEEDR |= ((uint32_t)(speed) << (m_pin * 2));
}

void GPIO :: af(PinAF f)
{
	uint8_t shift = (0x04 * (m_pin & 0x07));
	uint8_t index = m_pin >> 0x03;
	
	m_port->AFR[index] &= ~(GPIO_AFRL_AFRL0 << shift);
	m_port->AFR[index] |= (uint32_t)f << shift;
}

uint32_t GPIO :: port(void)
{
	return (uint32_t)m_port;
}

uint32_t GPIO :: mask(void)
{
	return m_mask;
}

uint8_t GPIO :: pin(void)
{
	return m_pin;
}

void GPIO :: write(uint32_t value)
{
	if(value == 0) m_port->BRR = m_mask;
	else m_port->BSRR = m_mask;
}

uint32_t GPIO :: read(void)
{
	return ((m_port->IDR & m_mask) ? 1 : 0);
}

GPIO :: operator uint32_t()
{
	return this->read();
}
