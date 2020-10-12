/*!
 * \file Memory.cpp
 * \brief Memory API.
 * \author Rémi.Debord
 * \version 1.0
 * \date 03 janvier 2017
 *
 * Memory library (Read and write on flash memory).
 *
 * Write operation: ~22ms (with page erase), else ~120us
 * Read operation: ~2us
 *
 * How to use: Read from memory at startup, and store ALL data periodically
 *
 */

#include "Memory.h"


Memory :: Memory(uint8_t page)
{
	// Page shall be in range [0:63]
	m_ptr = (uint8_t*)((page * FLASH_PAGE_SIZE) + FLASH_BASE);
	
	// Clear status register
	FLASH->SR = 0x0F;
}

void Memory :: unlock(void)
{
	if((FLASH->CR & FLASH_CR_LOCK) != 0)
	{
		// Keys sequence
		FLASH->KEYR = FLASH_FKEY1;
		FLASH->KEYR = FLASH_FKEY2;
	}
}

void Memory :: lock(void)
{
	FLASH->CR |= FLASH_CR_LOCK;
}

void Memory :: erase(uint32_t address)
{
	// Unlock flash
	Memory::unlock();
		
	// Wait flash
	while(Memory::busy());
	FLASH->SR = 0xFF;

	FLASH->CR |= FLASH_CR_PER;	// Page erase
	FLASH->AR = address;				// Page flash address
	FLASH->CR |= FLASH_CR_STRT;	// Start
	
	// Wait flash
	while(Memory::busy());
	
	FLASH->CR &= ~FLASH_CR_PER;	// Page erase
	
	// Lock flash
	Memory::lock();
}

uint8_t Memory :: busy(void)
{
	return (FLASH->SR & FLASH_SR_BSY);
}

uint8_t Memory :: write(uint32_t address, uint8_t* data, uint16_t length)
{
	uint8_t buffer[FLASH_PAGE_SIZE] = {0};
	uint8_t result = 1;
	uint16_t i = 0;
	
	uint16_t* ptr = (uint16_t*)&buffer[0];

	if((address + length) < FLASH_PAGE_SIZE)
	{
		// Get page content
		for(i = 0; i < FLASH_PAGE_SIZE; i++)
		{
			buffer[i] = m_ptr[i];
		}
		
		// Erase page
		this->erase((uint32_t)m_ptr);
		
		// Unlock flash
		Memory::unlock();
		
		// Insert data into the page content
		for(i = 0; i < length; i++)
		{
			buffer[address] = data[i];
			address++;
		}
		
		// Write page
		FLASH->CR |= FLASH_CR_PG; // Programming page
		
		for(i = 0; i < (FLASH_PAGE_SIZE >> 1); i++)
		{
			*((uint16_t*)m_ptr+i) = *(ptr+i);
			while(Memory::busy());
		}
		
		FLASH->CR &= ~FLASH_CR_PG;
		
		// Lock flash
		Memory::lock();
	}
	else
	{
		result = 0;
	}
	
	return result;
}

void Memory :: read(uint16_t address, uint8_t* data, uint16_t length)
{
	uint16_t i = 0;
	
	for(i = 0; i < length; i++)
	{
		data[i] = m_ptr[address];
		address++;
	}
}
