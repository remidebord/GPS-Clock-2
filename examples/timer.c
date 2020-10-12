#include "main.h"

Timer timer(TIM2);
DigitalOut led(PC_8);
uint32_t begin, end, elapsed;
 
int main(void)
{
	while(1)
	{
		timer.start();
	
		begin = timer.read_us();
	
		Delay(1000);
		
		led = !led;
	
		end = timer.read_us();
	
		elapsed = end - begin;
	}
}
