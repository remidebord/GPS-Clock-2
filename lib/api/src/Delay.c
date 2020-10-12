#include "Delay.h"

void Delay(uint32_t value)
{
	uint32_t tickstart = 0;
	
	tickstart = SysTick_Value();
	
	while((SysTick_Value() - tickstart) < value);
}
