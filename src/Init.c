#include "Init.h"
#include "Config.h"
#include "Callbacks.h"

void init(void)
{
	// Peripherals initilization are done in constructors before main() execution !
	
	// Callback executed every 1 ms (1000 us)
	ticker.attach_us(&tickMCU, 1000);
	
	// Callback square wave (DS3231)
	squareWave.risefall(tickRTC);
	
	// Add seven segment into a group
	sevenSegGroup.add(&sevenSeg0);
	sevenSegGroup.add(&sevenSeg1);
	sevenSegGroup.add(&sevenSeg2);
	sevenSegGroup.add(&sevenSeg3);
	
	// InterruptIn callback
	encoderRightChA.risefall(&EncoderRightirqA);
	encoderRightChB.risefall(&EncoderRightirqB);
	encoderLeftChA.risefall(&EncoderLeftirqA);
	encoderLeftChB.risefall(&EncoderLeftirqB);
						
	// Attach brightness variable to encoder
	encoderLeft.attach(brightness.value(), BRIGHTNESS_MAX);
	
	// Set encoder circular
	encoderLeft.circular(1);
	
	// Insert item into menu
	menu.insert(&altitude);
	menu.insert(&satellites);
	menu.insert(&fix);
	menu.insert(&temperature);
	menu.insert(&clock);
	
	// Get current item
	menu.current(&item);
	
	// Set brightness
	brightness.set(100);
	
	// Dots ON
	dots = 1;
}
