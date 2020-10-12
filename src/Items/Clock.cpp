#include "Clock.h"
#include "Config.h"

static const uint8_t name[4] = {'T', 'I', 'M', 'E'};
static uint8_t state = ITEM_INIT;
static uint8_t substate = 0;
static uint16_t timer = ITEM_INIT_TIME;
static Time t;

void clock_init(void)
{
	state = ITEM_INIT;
	timer = ITEM_INIT_TIME;
	dots = 0;
}

void clock_main(void)
{
	float latitude = 0;
	float longitude = 0;
	uint8_t buffer[4] = {0};
	uint8_t tmp = 0;
	
	switch(state)
	{
		case ITEM_INIT:
			// Display menu name
			sevenSegGroup.set((uint8_t*)&name[0], sizeof(name));
		
			// Dots OFF
			dots = 0;
		
			if(timer == 0) state = ITEM_MAIN;
			break;
		
		case ITEM_MAIN:			
			// Get time from DS3231 RTC
			ds3231.get(&t);
			break;
		
		case ITEM_SUB1:
			switch(substate)
			{
				case 0:
					if(buttonLeft.get() == SHORT_PRESS)
					{
						// Attach minutes to right encoder
						encoderRight.attach(&t.hours, 24);
							
						sevenSegGroup.clear();
							
						// Hours segments blinking (500 ms)
						sevenSegGroup.blink(0, 500);
						sevenSegGroup.blink(1, 500);
						
						substate = 1;
					}
					break;
				
				case 1:
					if(buttonLeft.get() == SHORT_PRESS)
					{
						encoderRight.attach(0, 0);
						
						// Set RTC time (ds3231)
						ds3231.set(&t);
							
						// Stop blinking
						sevenSegGroup.clear();
						
						state = ITEM_MAIN;
						substate = 0;
					}
					break;
			}
			break;
			
		case ITEM_SUB2:
			tmp = gnss.fix();
		
			gnss.time(&t);
			
			// GPS fix ?
			if(tmp != 0)
			{
				tz.offset(latitude, longitude);
			}
			
			ds3231.set(&t);
			
			if(timer == 0)
			{
				state = ITEM_MAIN;
			}
			break;
		
		default: break;
	}
	
	// Display time ?
	if((state != ITEM_INIT) && (state != ITEM_SUB2))
	{
		// Formatting
		tmp = bin2bcd(t.hours);
			
		buffer[0] = (tmp >> 4) + '0';
		buffer[1] = (tmp & 0x0F) + '0';
			
		tmp = bin2bcd(t.minutes);
			
		buffer[2] = (tmp >> 4) + '0';
		buffer[3] = (tmp & 0x0F) + '0';
			
		// Set seven segment display
		sevenSegGroup.set(&buffer[0], sizeof(buffer));
	}
}

void clock_sub1(void)
{
	// Dots ON
	dots = 1;
	
	// Attach minutes to right encoder
	encoderRight.attach(&t.minutes, 60);
					
	// Minutes segments blinking (500 ms)
	sevenSegGroup.blink(2, 500);
	sevenSegGroup.blink(3, 500);
	
	state = ITEM_SUB1;
	substate = 0;
}

void clock_sub2(void)
{
	uint8_t buffer[4] = {'-', '-', '-', '-'};
	
	// Set seven segment display
	sevenSegGroup.set(&buffer[0], sizeof(buffer));
	
	timer = CLOCK_SYNC_TIME;
	state = ITEM_SUB2;
}

void clock_timer(void)
{
	if(timer) timer--;
}

uint8_t clock_state(void)
{
	return state;
}
