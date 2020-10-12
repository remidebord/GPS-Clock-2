#include "main.h"

Power power;

DigitalOut led1(PC_9);
DigitalOut led2(PC_8);

InterruptIn button(PA_0);

void toggle(void)
{
	led2 = !led2;
}

int main(void)
{
	uint8_t i = 0;
	
	button.rise(&toggle);
	
	while(1)
	{				
		// 5 sec ?
		if(i >= 50)
		{
			i = 0;
			led1 = 0;
			
			// Go to stop mode (Wakeup: any EXTI line)
			power.stop();
		}
		
		led1 = !led1;
		Delay(100);
		
		i++;
	}
}