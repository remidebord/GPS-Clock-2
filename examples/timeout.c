#include "main.h"

Timeout timeout(TIM17);

DigitalOut led1(PC_8);
DigitalOut led2(PC_9);

void blink(void)
{
	led2 = !led2;
	
	//timeout.start();
}

int main(void)
{
	timeout.attach_ms(&blink, 2000);

	led1 = 1;
	led2 = 0;
	
	timeout.start();
	
	while(1)
	{
		
	}
}
