/*!
 * \file SevenSeg.cpp
 * \brief Seven segment API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 13 juin 2015
 *
 * Seven segment library.
 *
 */

#include "SevenSeg.h"
#include "AsciiMap.h"

/*!
 *  \brief Constructor
 *
 *  SevenSeg constructor.
 *
 */

SevenSeg :: SevenSeg(DigitalOut* segments, DigitalOut* digit)
{
	uint8_t i = 0;
	
	for(i = 0; i < MAX_SEGMENTS; i++)
	{
		m_segment[i] = segments++;
	}
	
	m_digit = digit;
	
	// !important
	m_digit->type(Open_Drain);
}

/*!
 *  \brief Segments
 *
 *  Set segments value.
 *
 *  (msb) dot g f e d c b a (lsb)
 *
 *  \param c : segments values
 */

void SevenSeg :: segments(uint8_t value)
{
	uint8_t i = 0;
	uint8_t tmp = 0;
	
	for(i = 0; i < MAX_SEGMENTS; i++)
	{
		tmp = ((value & (0x01 << i)) != 0) ? 1 : 0;
			
		m_segment[i]->write(tmp);
	}
}

/*!
 *  \brief Set
 *
 *  Set segments from an ASCII characters.
 *
 *  \param c : ASCII character
 */

void SevenSeg :: set(uint8_t c)
{	
	if(c < sizeof(digitAsciiMap))
	{
		this->segments(digitAsciiMap[c]);
	}
}

/*!
 *  \brief Set
 *
 *  Set segments from an integer value.
 *
 *  \param number : integer
 */

void SevenSeg :: set(uint32_t number)
{
	uint8_t tmp = (bin2bcd(number) & 0x0F) + '0';
	
	this->set(tmp);
}

/*!
 *  \brief On
 *
 *  Switch the digit ON.
 *
 */

void SevenSeg :: on(void)
{
	m_digit->write(0);
}

/*!
 *  \brief Off
 *
 *  Switch the digit OFF.
 *
 */

void SevenSeg :: off(void)
{
	m_digit->write(1);
}

/*!
 *  \brief Constructor
 *
 *  SevenSeg constructor.
 *
 */

SevenSegGroup :: SevenSegGroup(void)
{
	// Init value(s)
	m_size = 0;
	m_inc = 0;
	m_timer = 0;
}

/*!
 *  \brief Update
 *
 *  Manage digits display.
 *
 */

void SevenSegGroup :: update(void)
{	
	uint8_t tmp = 0;
	uint8_t i = 0;
	
	// Refresh ?
	if(m_timer == 0)
	{
		m_timer = m_mux;
		
		// SevenSeg object inserted ?
		if(m_size > 0)
		{
			// Special effects using segments ?
			if(m_effect.reload)
			{
				m_digit[m_inc]->segments(m_buffer[m_inc]);
			}
			// Display ASCII characters ?
			else
			{
				m_digit[m_inc]->set(m_buffer[m_inc]);
			}
			
			// Switch digits (Only 1 turn ON)
			for(i = 0; i < m_size; i++)
			{
				// Digit to switch ON ?
				if(i == m_inc)
				{
					// Digit selected for blinking ? and blink effect activated ?
					if(((m_blink.select & (0x01 << i)) != 0) && (m_blink.flag))
					{
						m_digit[m_inc]->set((uint8_t)0x00);
					}
					
					m_digit[i]->on();
				}
				// Switch OFF others digits
				else
				{
					m_digit[i]->off();
				}
			}
			
			m_inc++;
						
			// Avoid modulo !
			m_inc = (m_inc >= m_size) ? 0 : m_inc;
		}
	}
	
	// Blink ?
	if((m_blink.timer == 0) && m_blink.reload)
	{
		// Reload timer
		m_blink.timer = m_blink.reload;
		
		// Invert
		m_blink.flag = m_blink.flag ^ 1;
	}
	
	// Scroll ?
	if((m_scroll.timer == 0) && m_scroll.reload)
	{
		// Reload timer
		m_scroll.timer = m_scroll.reload;
		
		// Save the first character
		tmp = m_buffer[0];
		
		// Move the characters forward
		for(i = 0; i < m_size; i++)
		{
			m_buffer[i] = m_buffer[i+1];
		}
		
		// Load the character previously saved at the last index
		m_buffer[m_size - 1] = tmp;
	}
	
	// Effect ?
	if((m_effect.timer == 0) && m_effect.reload)
	{
		// Reload timer
		m_effect.timer = m_effect.reload;
		
		m_effect.flag = (m_effect.flag + 1) % m_effect.length;
		
		for(i = 0; i < m_size; i++)
		{
			m_buffer[i] = m_effect.buffer[(m_effect.flag+(m_effect.shift*i))%m_effect.length];
		}
	}
}

/*!
 *  \brief Add
 *
 *  Add a digit (seven segment) to the group.
 *
 *  \param digit: Seven segment object
 */

void SevenSegGroup :: add(SevenSeg* digit)
{
	m_digit[m_size++] = digit;
	
	m_mux = REFRESH_TIME / m_size;
}

/*!
 *  \brief Set (string)
 *
 *  Set an ASCII string to display.
 *
 *  \param buffer: ASCII string
 *  \param length: ASCII string length
 */

void SevenSegGroup :: set(uint8_t* buffer, uint32_t length)
{
	uint8_t i = 0;
	
	length = (length > m_size) ? m_size : length;
	
	for(i = 0; i < length; i++)
	{
		m_buffer[i] = buffer[i];
	}
}

/*!
 *  \brief Blink
 *
 *  All the seven segment in the group blink at the specified delay.
 *
 *  \param delay : blink delay (ms)
 */

void SevenSegGroup :: blink(uint32_t delay)
{
	if(m_blink.reload != delay)
	{
		m_blink.select = 0xFFFF;
		
		m_blink.reload = delay;
		m_blink.flag = 0;
		
		m_blink.timer = m_blink.reload;
	}
}

/*!
 *  \brief Blink
 *
 *  Select one seven segment to blink at the specified delay.
 *
 *  \param select : 0 to n seven segment
 *  \param delay : blink delay (ms)
 */

void SevenSegGroup :: blink(uint8_t select, uint32_t delay)
{
	m_blink.select |= 0x01 << select;
	
	if(m_blink.reload != delay)
	{		
		m_blink.reload = delay;
		m_blink.flag = 0;
		
		m_blink.timer = m_blink.reload;
	}
}

/*!
 *  \brief Scroll
 *
 *  Scroll text displayed on the seven segments (right to left)
 *
 *  \param ms : scroll delay (ms)
 */

void SevenSegGroup :: scroll(uint32_t delay)
{
	if(m_scroll.reload != delay)
	{
		m_scroll.reload = delay;
		m_scroll.flag = 0;
		
		m_scroll.timer = m_scroll.reload;
	}
}

/*!
 *  \brief Effect
 *
 *  User specified effect (address directly the segments).
 *
 *  \param buffer : array with special value
 *  \param length : array length
 *  \param shift : shift
 *  \param delay : delay in ms
 */

void SevenSegGroup :: effect(uint8_t* buffer, uint32_t length, uint32_t shift, uint32_t delay)
{
	if(m_effect.reload != delay)
	{
		m_effect.buffer = buffer;
		m_effect.length = length;
		m_effect.shift = shift;
		
		m_effect.reload = delay;
		m_effect.flag = 0;
		
		m_effect.timer = m_effect.reload;
	}
}

/*!
 *  \brief Clear
 *
 *  Disable any effect.
 *
 */

void SevenSegGroup :: clear(void)
{
	m_blink.select = 0;
	m_blink.reload = 0;
	m_scroll.reload = 0;
	m_effect.reload = 0;
}

/*!
 *  \brief Manage SevenSegGroup base time 
 *
 *  Note: Place it into a ticker callback of 1 ms
 *
 */

void SevenSegGroup :: timer(void)
{
	if(m_timer) m_timer--;
	if(m_blink.timer) m_blink.timer--;
	if(m_scroll.timer) m_scroll.timer--;
	if(m_effect.timer) m_effect.timer--;
}
