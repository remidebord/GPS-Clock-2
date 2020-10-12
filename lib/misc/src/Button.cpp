/*!
 * \file Button.cpp
 * \brief Button API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 05 june 2017
 *
 * Button library.
 *
 */

#include "Button.h"

Button :: Button(PinName pin): DigitalOut(pin)
{
	m_value = 0;
	m_state = WAIT_PRESS;
	m_timer = 0;
}

/*!
 *  \brief Update
 *
 *  Manage button state.
 *
 */

void Button :: update(void)
{
	uint8_t tmp = 0;
	
	// Get input level
	tmp = this->read();
	
	// Manage button states
	switch(m_state)
	{
		case WAIT_PRESS:
			// Rising edge ?
			if((tmp != 0) && (m_value == 0))
			{
				// Debounce
				if(m_timer >= DEBOUNCE_TIME)
				{
					m_state = WAIT_RELEASE;
				}
			}
			else m_timer = 0;
			break;
		
		case WAIT_RELEASE:
			// Falling edge ?
			if(tmp == 0)
			{
				m_value = SHORT_PRESS;
			}
			else
			{
				// Button still pressed ?
				if(m_timer >= LONG_PRESS_TIME)
				{
					m_value = LONG_PRESS;
				}
			}
			break;
			
		default:
			m_value = 0;
			
			if(tmp == 0)
			{
				m_timer = 0;
				m_state = WAIT_PRESS;
			}
			break;
	}
	
	// Go to default state ?
	if(m_value != 0)
	{
		m_state = WAIT_RELEASE + 1;
	}
}

uint8_t Button :: get(void)
{
	return m_value;
}

void Button :: timer(void)
{
	if(m_timer < LONG_PRESS_TIME) m_timer++;
}
