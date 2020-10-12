#include "main.h"
	
Ticker tick1(TIM17);
DigitalOut led1(PC_8);

Ticker tick2(TIM16);
DigitalOut led2(PC_9);

void blink1(void)
{
	led1 = !led1;
}

void blink2(void)
{
	led2 = !led2;
}

int main(void)
{
	tick1.attach_ms(&blink1, 500);
	tick2.attach_ms(&blink2, 2000);
	
	while(1)
	{
	
	}
}