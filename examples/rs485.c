#include "main.h"

// RX, TX, DE
Serial rs485(USART2, PA_3, PA_2, PA_1);

int main(void)
{	
	while(1)
	{
		rs485.write((uint8_t*)"Hello world!", 12);
	
		Delay(1000);
	}
}
