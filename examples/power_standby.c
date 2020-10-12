#include "main.h"

Power power;

DigitalOut led1(PC_9);

int main(void)
{
	uint8_t i = 0;
	
	while(1)
	{				
		// 5 sec ?
		if(i >= 50)
		{
			i = 0;
			led1 = 0;
			
			// Go to standby mode (Wakeup pin: PA_0)
			power.standby(PA_0);
		}
		
		led1 = !led1;
		Delay(100);
		
		i++;
	}
}