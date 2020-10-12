#include "main.h"

Ticker tick(TIM16);

Serial serial(USART1, PA_10, PA_9);

GNSS gnss(&serial);

float latitude = 0;
float longitude = 0;
Time t;
Date d;

int main(void)
{
	// Callback executed every 1 ms (1000 us)
	tick.attach_us(&GlobalBaseTime, 1000);
	
  while (1)
  {
		gnss.update();
		
		gnss.time(&t);
		gnss.date(&d);
		
		if(gnss.fix())
		{			
			gnss.position(&latitude, &longitude);
		}
	}
}

void GlobalBaseTime(void)
{
	
}
