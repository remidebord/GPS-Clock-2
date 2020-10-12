#include "main.h"

#define FREQUENCY 1000 // 1kHz

PwmOut pwm1(PC_9, FREQUENCY, TIM3, Channel_4);
PwmOut pwm2(PC_8, FREQUENCY, TIM3, Channel_3);

int main(void)
{
	uint8_t i = 0;
	uint8_t dir = 0;
	
	while(1)
	{
		pwm1 = i;
		pwm2 = PWMOUT_DUTYCYCLE_MAX - i;
		
		// Increment/decrement
		i = (dir == 0) ? (i + 1) : (i - 1);
		
		if((i >= PWMOUT_DUTYCYCLE_MAX) || (i == 0)) dir = ~dir;
		
		Delay(10);
	}
}
