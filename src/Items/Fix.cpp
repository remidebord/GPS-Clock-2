#include "Fix.h"
#include "Config.h"

static const uint8_t name[4] = {' ', 'F', 'I', 'X'};
static uint8_t state = ITEM_INIT;
static uint16_t timer = ITEM_INIT_TIME;

void fix_init(void)
{
	state = ITEM_INIT;
	timer = ITEM_INIT_TIME;
	dots = 0;
}

void fix_main(void)
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
			tmp = gnss.fix();
		
			if(tmp == 0) buffer[1] = 'N';
		
			buffer[2] = 'O';
			buffer[3] = 'K';
		
			// Set seven segment display
			sevenSegGroup.set(&buffer[0], sizeof(buffer));
			break;
		
		default: break;
	}
}

void fix_sub1(void)
{
	// Nothing to do !
}

void fix_sub2(void)
{
	// Nothing to do !
}

void fix_timer(void)
{
	if(timer) timer--;
}

uint8_t fix_state(void)
{
	return state;
}
