#include "main.h"

Clock clock;

DigitalOut led1(PC_9);

InterruptIn pushButton(PA_0);

Time time;
Date date;

void Push(void)
{	
	time.hours = 19;
	time.minutes = 33;
	time.seconds = 0;
	
	clock.set(&time);
	
	date.year = 17;
	date.month = 1;
	date.day = 13;
	
	clock.set(&date);
	
	led1 = !led1;
}

int main(void)
{
	pushButton.rise(&Push);

	while(1)
	{
		clock.get(&time);
		clock.get(&date);
	}
}
