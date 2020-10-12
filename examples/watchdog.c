#include "main.h"
	
DigitalOut led1(PC_8);
	
InterruptIn pushButton(PA_0);

Watchdog Watchdog(2000);

void Push(void)
{
	while(1);
}

int main(void)
{	
	pushButton.rise(&Push);
	
	while(1)
	{
		led1 = !led1;
		Delay(100);
		
		Watchdog.reload();
	}
}