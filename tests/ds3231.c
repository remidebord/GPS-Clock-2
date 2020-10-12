#include "main.h"

Ticker tick(TIM16);

DigitalIn pushButton(PA_2);

I2C	i2c(I2C2, PF_7, PF_6);

DS3231 ds3231(&i2c);

Date date;
Time time;
uint8_t temp;

int main(void)
{
	// Callback executed every 1 ms (1000 us)
	tick.attach_us(&GlobalBaseTime, 1000);
	
  while (1)
  {
		ds3231.update();
		
		ds3231.get(&time);
		ds3231.get(&date);
		
		temp = ds3231.temperature();
		
		if(pushButton)
		{
			time.seconds = 50;
			time.minutes = 59;
			time.hours = 13;
			
			date.day = 12;
			date.month = 02;
			date.year = 17;
			
			ds3231.set(&time);
			ds3231.set(&date);
		}
	}
}

void GlobalBaseTime(void)
{
	ds3231.timer();
}
