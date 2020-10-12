#include "main.h"
	
DigitalOut led1(PC_8);
DigitalOut led2(PC_9);
	
// DigitalIn pushButton(PA_0);
InterruptIn pushButton(PA_0);

void Push(void)
{
	led2 = !led2;
}

int main(void)
{
	pushButton.rise(&Push);
	
	while(1)
	{
		led1 = !led1;
		Delay(100);
		
// 		if(pushButton) led2 = 1;
// 		else led2 = 0;
	}
}