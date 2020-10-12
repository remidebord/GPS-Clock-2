#include "main.h"

#define SI7021_ADDR (0x40 << 1)

#define SI7021_TEMPERATURE_CMD 0xF3

#define SI7021_TEMP_COEF1 175
#define SI7021_TEMP_COEF2 46

I2C	i2c(I2C1, PB_7, PB_6);

uint16_t temperature = 0;

int main(void)
{
	uint8_t buffer[8] = {0};	
	uint8_t state = 0;
	uint32_t tmp = 0;
	
	while(1)
	{
		switch(state)
		{
			case 0:
				buffer[0] = SI7021_TEMPERATURE_CMD;
			
				if(i2c.write(SI7021_ADDR, buffer, 1))
				{
					state++;
				}
				break;
			
			case 1:
				if(i2c.read(SI7021_ADDR, buffer, 2))
				{
					tmp = (buffer[0] << 8) + buffer[1];
					tmp = tmp * SI7021_TEMP_COEF1;
					tmp = tmp >> 16; // divide by 2^16 (65536)
					temperature = tmp - SI7021_TEMP_COEF2;
					
					state++;
				}
				break;
			
			case 2:
				Delay(1000);
				state = 0;
				break;
		}
	}
}
