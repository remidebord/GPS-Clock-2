#include "Satellites.h"
#include "Config.h"

static const uint8_t name[4] = {' ', 'S', 'A', 'T'};
static uint8_t state = ITEM_INIT;
static uint16_t timer = ITEM_INIT_TIME;

void satellites_init(void)
{
	state = ITEM_INIT;
	timer = ITEM_INIT_TIME;
	dots = 0;
}

void satellites_main(void)
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
			// Get number of satellites from GPS
			tmp = gnss.satellites();
		
			// Formatting
			buffer[3] = (bin2bcd(tmp) & 0x0F) + '0';
			
			if(tmp >= 10) buffer[2] = (bin2bcd(tmp) >> 4) + '0';
		
			// Set seven segment display
			sevenSegGroup.set(&buffer[0], sizeof(buffer));
			break;
		
		default: break;
	}
}

void satellites_sub1(void)
{
	// Nothing to do !
}

void satellites_sub2(void)
{
	// Nothing to do !
}

void satellites_timer(void)
{
	if(timer) timer--;
}

uint8_t satellites_state(void)
{
	return state;
}
