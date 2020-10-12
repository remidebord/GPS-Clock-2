/*!
 * \file Brightness.cpp
 * \brief Brightness API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 28 janvier 2017
 *
 * Brightness library.
 *
 */

#include "Brightness.h"

Brightness :: Brightness(PinName pin, uint32_t frequency, TIM_TypeDef* timer, TimerChannel channel) :  PwmOut(pin, frequency, timer, channel)
{
	this->set(BRIGHTNESS_MAX);
}

void Brightness :: set(uint8_t value)
{
	m_value = value;
	
	if(m_value >= BRIGHTNESS_MAX) m_value = BRIGHTNESS_MAX;
	
	this->write(cie[m_value]);
}

void Brightness :: set(void)
{
	if(m_value >= BRIGHTNESS_MAX) m_value = BRIGHTNESS_MAX;
	
	this->write(cie[m_value]);
}


uint8_t* Brightness :: value(void)
{
	return &m_value;
}
