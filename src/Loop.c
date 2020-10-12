#include "Loop.h"
#include "Config.h"

void loop(void)
{
	// Manage encoders
	encoderLeft.update();
	encoderRight.update();
		
	// Manage push buttons
	buttonLeft.update();
	buttonRight.update();
		
	// Manage GPS receiver
	gnss.update();
		
	// Manage DS3231 RTC
	ds3231.update();
		
	// Manage short press
	if(buttonRight.get() == SHORT_PRESS)
	{
		menu.next();
	}
	else if(buttonLeft.get() == SHORT_PRESS)
	{
		menu.previous();
	}
		
	// Manage long press
	if(item->state() == ITEM_MAIN)
	{
		if(buttonRight.get() == LONG_PRESS)
		{
			item->sub1();
		}
		else if(buttonLeft.get() == LONG_PRESS)
		{
			item->sub2();
		}
	}
		
	// Get current item
	menu.current(&item);
		
	// Current item main routine
	item->main();
		
	// Set brightness
	brightness.set();	
}
