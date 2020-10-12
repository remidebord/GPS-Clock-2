#include "main.h"
	
Ticker tick(TIM16);
DigitalOut led1(PC_8);
DigitalOut led2(PC_9);

void blink(void)
{
	led1 = !led1;
}

int main(void)
{
	tick.attach_ms(&blink, 500);
	
	while(1)
	{
		led2 = !led2;
		Delay(100);
	}
}
