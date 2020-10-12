#include "Altitude.h"
#include "Config.h"

static const uint8_t name[4] = {' ', 'A', 'L', 'T'};
static uint8_t state = ITEM_INIT;
static uint16_t timer = ITEM_INIT_TIME;

void altitude_init(void)
{
	state = ITEM_INIT;
	timer = ITEM_INIT_TIME;
	dots = 0;
}

void altitude_main(void)
{
	uint8_t buffer[4] = {0};
	uint16_t tmp = 0;
	
	switch(state)
	{
		case ITEM_INIT:
			// Display menu name
			sevenSegGroup.set((uint8_t*)&name[0], sizeof(name));
		
			if(timer == 0) state = ITEM_MAIN;
			break;
		
		case ITEM_MAIN:		
			// Get altitude from GPS
			tmp = gnss.altitude();
		
			// Formatting
			buffer[2] = bin2bcd((tmp % 10)) + '0';
			
			if(tmp >= 10) buffer[1] = bin2bcd((tmp / 10) % 10) + '0';
			if(tmp >= 100) buffer[0] = bin2bcd((tmp / 100) % 10) + '0';
		
			buffer[3] = 'M';
		
			// Set seven segment display
			sevenSegGroup.set(&buffer[0], sizeof(buffer));
			break;
		
		default: break;
	}
}

void altitude_sub1(void)
{
	// Nothing to do !
}

void altitude_sub2(void)
{
	// Nothing to do !
}

void altitude_timer(void)
{
	if(timer) timer--;
}

uint8_t altitude_state(void)
{
	return state;
}
