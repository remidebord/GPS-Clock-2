#include "Common.h"

uint8_t bin2bcd(uint8_t bin)
{
	uint16_t tmp = 0;
	uint8_t bcd = 0;
	
	// Divide by 10
	tmp = bin >> 1;
	tmp = tmp & 0x7F;
	tmp = tmp * 0x67;
	tmp = tmp >> 9;
	
	// Get tens digit
	tmp = tmp & 0x0F;
	
	// Shift tens digit and add modulo result
	bcd = (tmp << 4) + (bin - (tmp * 10));
	
	return bcd;
}

uint8_t bcd2bin(uint8_t bcd)
{
	uint8_t bin = 0;
	
	bin = (bcd >> 4) * 10;
	bin += (bcd & 0x0F);
	
	return bin;
}
