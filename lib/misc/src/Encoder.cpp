/*!
 * \file Encoder.cpp
 * \brief Rotary encoder API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 01 juillet 2015
 *
 * Rotary encoder library (!= motor encoder).
 *
 * Thanks to Mauro Alfieri (http://www.mauroalfieri.it/elettronica/rover-5-usare-lencoder.html)
 * and Gavin Kistner (http://phrogz.net/round-to-nearest-via-modulus-division)
 *
 */

#include "Encoder.h"

const int8_t encSequence[4][4] = {0, -1,  1,  2, 1,  0,  2, -1, -1,  2,  0,  1, 2,  1, -1,  0};

/*!
 *  \brief Constructor
 *
 *  Encoder constructor.
 *
 *  \param A : InterruptIn object address (channel A)
 *  \param A : InterruptIn object address (channel B)
 */
																		 
Encoder :: Encoder(InterruptIn* A, InterruptIn* B)
{
	m_A = A;
	m_B = B;
	
	m_A->pull(Pull_Up);
	m_B->pull(Pull_Up);
	
	m_value = (m_B->read() << 1) + m_A->read();
	m_old = m_value;
	
	m_value = 0;
	m_circular = 1;
}

/*!
 *  \brief Update
 *
 *  Manage variable (Optionnal)
 *
 */

void Encoder :: update(void)
{
		m_n = this->get();
		
		// Change ?
		if(m_n != m_p)
		{
			// Pointer referenced ?
			if(m_variable != 0)
			{
				// Increment ?
				if(m_n > m_p)
				{
					// Circular ?
					if(m_circular)
					{
						*m_variable = (*m_variable + 1) % m_limit;
					}
					else
					{
						// variable don't reach limit ?
						if((*m_variable + 1) < m_limit)
						{
							*m_variable = *m_variable + 1;
						}
					}
				}
				// Decrement ?
				else
				{
					// Circular ?
					if(m_circular)
					{
						*m_variable = ((*m_variable - 1) < 0) ? (m_limit - 1) : (*m_variable - 1);
					}
					else
					{
						// variable don't reach limit ?
						if((*m_variable - 1) >= 0)
						{
							*m_variable = *m_variable - 1;
						}
					}
				}
			}
			
			m_p = m_n;
		}
}

/*!
 *  \brief Circular
 *
 *  Enable circular attached variable.
 *  Permit to reach another value when m_variable is at m_limit.
 *
 *  \param x : variable address
 */

void Encoder :: circular(uint8_t x)
{
	m_circular = (x != 0) ? 1 : 0;
}

/*!
 *  \brief Attach
 *
 *  Attach a variable to the encoder, with a specified limit
 *
 *  \param var : variable address
 * \param limit : variable limit
 */

void Encoder :: attach(uint8_t* variable, uint8_t limit)
{
	m_variable = variable;
	m_limit = limit;
}

/*!
 *  \brief Read
 *
 *  Read accumulated value.
 *
 *  \return counter: accumulated value
 */

uint32_t Encoder :: get(void)
{
	return (Encoder :: round(m_accum, 4) / 4);
}

/*!
 *  \brief IRQ
 *
 *  Need to be placed in InterruptIn channel A and B callback.
 *
 */

void Encoder :: irq(void)
{	
	m_value = (m_B->read() << 1) + m_A->read();
	
	// New value ?
	if(m_value != m_old)
	{
		// Sequence valid ? (!= 2)
		if(encSequence[m_value][m_old] != 2)
		{
			m_accum += encSequence[m_value][m_old];
			
			m_old = m_value;
		}
	}
}

/*!
 *  \brief round
 *
 *  Round a number to the nearest value.
 *
 * \param number: number to round
 * \param divisor: number divisor
 */

int32_t Encoder :: round(int32_t number, int32_t divisor)
{
	int32_t half = divisor / 2;
	int32_t result = 0;
	
	if(number < 0) half = -half;
	
	result = ((number + half) - ((number + half) % divisor));
	
	return result;
}
