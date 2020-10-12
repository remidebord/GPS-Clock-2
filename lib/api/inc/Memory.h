#ifndef __MEMORY_H
#define __MEMORY_H

/* includes ---------------------------------------------------------------- */
#include "common.h"

/* defines ----------------------------------------------------------------- */
#define FLASH_PAGE_SIZE 1024 // 1kB

/* class ------------------------------------------------------------------- */
class Memory
{
	private:
		
		uint8_t* m_ptr;
	
	public:
		
		Memory(uint8_t page);
		
		static void unlock(void);
		static void lock(void);
		static void erase(uint32_t address);
		static uint8_t busy(void);
		
		uint8_t write(uint32_t address, uint8_t* data, uint16_t length);
		void read(uint16_t address, uint8_t* data, uint16_t length);
};

#endif
