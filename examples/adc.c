#include "main.h"
	
uint16_t value1 = 0;
uint16_t value2 = 0;
uint16_t value3 = 0;
uint16_t value4 = 0;
uint16_t value5 = 0;

AnalogIn ain1(PC_5); // Channel 15
AnalogIn ain2(PA_5); // Channel 5
AnalogIn ain3(PA_1); // Channel 1
AnalogIn ain4(PC_4); // Channel 14
AnalogIn ain5(PB_0); // Channel 8

int main(void)
{	
	while(1)
	{
		value1 = ain1.read_b();
		value2 = ain2.read_b();
		value3 = ain3.read_b();
		value4 = ain4.read_b();
		value5 = ain5.read_b();
	}
}