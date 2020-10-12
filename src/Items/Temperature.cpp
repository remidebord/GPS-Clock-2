#include "Temperature.h"
#include "Config.h"

static const uint8_t temp[4] = {'T', 'E', 'M', 'P'};
static uint8_t state = ITEM_INIT;
static uint16_t timer = ITEM_INIT_TIME;

void temperature_init(void)
{
	state = ITEM_INIT;
	timer = ITEM_INIT_TIME;
	dots = 0;
}

void temperature_main(void)
{
	uint8_t buffer[4] = {0};
	int8_t tmp = 0;
	
	switch(state)
	{
		case ITEM_INIT:
			// Display menu name
			sevenSegGroup.set((uint8_t*)&temp[0], sizeof(temp));
			
			if(timer == 0) state = ITEM_MAIN;
			break;
		
		case ITEM_MAIN:
			// Get temperature from DS3231 RTC
			tmp = ds3231.temperature();
			
			// Formatting
			if((tmp > 9) || (tmp < -9))
			{
				if(tmp < 0) buffer[0] = '-';
					
				buffer[1] = (bin2bcd(tmp) >> 4) + '0';
			}
			else
			{
				if(tmp < 0) buffer[1] = '-';
			}
				
			buffer[2] = (bin2bcd(tmp) & 0x0F) + '0';
			buffer[3] = 'C';
			
			// Set seven segment display
			sevenSegGroup.set(&buffer[0], sizeof(buffer));
			break;
		
		default: break;
	}
}

void temperature_sub1(void)
{
	// Nothing to do !
}

void temperature_sub2(void)
{
	// Nothing to do !
}

void temperature_timer(void)
{
	if(timer) timer--;
}

uint8_t temperature_state(void)
{
	return state;
}
