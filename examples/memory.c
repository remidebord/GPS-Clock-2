#include "main.h"
	
DigitalOut led1(PC_8);
DigitalOut led2(PC_9);
	
DigitalIn pushButton(PA_0);

Memory memory(62); // page 62 (0x0800F800)

uint8_t text[32] = "Hello World!";

int main(void)
{	
	uint16_t i = 0;
	
	while(1)
	{
		led1 = !led1;
		Delay(100);
		
		if(pushButton)
		{
			led2 = 1;
			memory.write(i, text, sizeof(text)); 
			i+=sizeof(text);
		}
		else led2 = 0;
	}
}