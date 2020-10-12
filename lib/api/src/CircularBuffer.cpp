/*!
 * \file CircularBuffer.cpp
 * \brief Circular buffer API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 08 janvier 2016
 *
 * Circular buffer library.
 *
 */

#include "CircularBuffer.h"

CircularBuffer :: CircularBuffer(uint8_t* buffer, uint16_t size)
{
	m_read = 0;
	m_write = 0;
	m_count = 0;
	
	m_buffer = buffer;
	m_size = size;
}

uint16_t CircularBuffer :: count(void)
{
	return m_count;
}

void CircularBuffer :: flush(void)
{
	m_read = 0;
	m_write = 0;
	m_count = 0;
}

void CircularBuffer :: put(uint8_t c)
{
	// Don't overwrite data not yet read !
	if(m_count < m_size)
	{
		// Set byte
		m_buffer[m_write] = c;
	
		// Increment write index and avoid buffer overflow
		m_write = ((m_write + 1) & (~m_size));
		
		m_count++;
	}
}

uint8_t CircularBuffer :: get(void)
{
	// Get byte
	uint8_t c = m_buffer[m_read];
	
	// Increment read index and avoid buffer overflow
	m_read = ((m_read + 1) & (~m_size));
	
	if(m_count) m_count--;
	
	return c;
}
