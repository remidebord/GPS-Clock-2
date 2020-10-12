#include "main.h"

uint8_t buffer[32] = "Hello World!";

// CS, SCK, MOSI, MISO
SPI spi(SPI2, PC_6, PB_13, PB_15, PB_14);

int main(void)
{
	uint8_t i = 0;
	
	while(1)
	{
		spi.cs(0);
		
		for(i = 0; i < 32; i++)
		{
			spi.write(buffer[i]);
		}
		
		spi.cs(1);
		
		Delay(1000);
	}
}
