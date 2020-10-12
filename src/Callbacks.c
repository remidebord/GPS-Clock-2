#include "Callbacks.h"
#include "Config.h"

void tickRTC(void)
{
	ds3231.irq();
	
	// Blink dots ?
	if((item == &clock) && (item->state() == ITEM_MAIN))
	{
		if(dots.read() != 0)
		{
			dots = 0;
		}
		else
		{
			dots = 1;
		}
	}
}

void tickMCU(void)
{	
	sevenSegGroup.timer();
	ds3231.timer();
	buttonLeft.timer();
	buttonRight.timer();
	
	// Manage display
	sevenSegGroup.update();		
	
	if(item != 0)
	{
		item->timer();
	}
}

void EncoderRightirqA(void)
{	
	encoderRight.irq();
}

void EncoderRightirqB(void)
{
	encoderRight.irq();
}

void EncoderLeftirqA(void)
{
	encoderLeft.irq();
}

void EncoderLeftirqB(void)
{
	encoderLeft.irq();
}
