#include "main.h"

const uint16_t Sine12bit[32] = {
	2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
	3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909, 
	599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};

uint16_t value = 0;

AnalogOut aout(PA_4);
AnalogIn ain(PC_5); // Channel 15

int main(void)
{
	uint8_t i;
	
	while(1)
	{
		for(i = 0; i < 32; i++)
		{
			aout = Sine12bit[i];
			value = ain.read_b();
			Delay(1000);
		}
	}
}