#include "Config.h"

// Ticker for base timer
Ticker ticker(TIM16);

// Segment array (a b c d e f g)
DigitalOut segment[] = {DigitalOut(PA_4), DigitalOut(PA_6), DigitalOut(PB_15), DigitalOut(PA_11),
                        DigitalOut(PA_12), DigitalOut(PA_5), DigitalOut(PB_14)};

												
// Digit array (d4 d3 d2 d1)
DigitalOut digit[] = {DigitalOut(PB_2), DigitalOut(PB_1), DigitalOut(PB_0), DigitalOut(PA_7)};

// Dots
DigitalOut dots(PA_8);

// One seven segment is composed of an array of segment and one digit pin
SevenSeg sevenSeg0(segment, &digit[0]);
SevenSeg sevenSeg1(segment, &digit[1]);
SevenSeg sevenSeg2(segment, &digit[2]);
SevenSeg sevenSeg3(segment, &digit[3]);

// Create group of seven segment
SevenSegGroup sevenSegGroup;

// InterruptIn (right encoder)
InterruptIn encoderRightChA(PA_0);
InterruptIn encoderRightChB(PA_1);

// InterruptIn (left encoder)
InterruptIn encoderLeftChA(PB_8);
InterruptIn encoderLeftChB(PB_9);

// Rotary encoders
Encoder encoderRight(&encoderRightChA, &encoderRightChB);
Encoder encoderLeft(&encoderLeftChA, &encoderLeftChB);	

// Pusb buttons on encoders
Button buttonRight(PA_2);
Button buttonLeft(PB_5);
	
// led brightness
Brightness brightness(PA_3, 1000, TIM15, Channel_2);

// Serial object
Serial serial(USART1, PA_10, PA_9);

// GPS
GNSS gnss(&serial);

// I2C object
I2C	i2c(I2C2, PF_7, PF_6);

// DS3231 square wave interrupt
InterruptIn squareWave(PB_4);

// RTC
DS3231 ds3231(&i2c, &squareWave);

// Timezone
Timezone tz;

// GPS clock menu (linked list)
Menu menu;

// Time and date global variable
Time time;
Date date;

// Items
Item clock = {&clock_init, &clock_main, &clock_sub1, &clock_sub2, &clock_timer, &clock_state, 0, 0};
Item temperature = {&temperature_init, &temperature_main, &temperature_sub1, &temperature_sub2, &temperature_timer,&temperature_state, 0, 0};
Item fix = {&fix_init, &fix_main, &fix_sub1, &fix_sub2, &fix_timer, &fix_state, 0, 0};
Item satellites = {&satellites_init, &satellites_main, &satellites_sub1, &satellites_sub2, &satellites_timer, &satellites_state, 0, 0};
Item altitude = {&altitude_init, &altitude_main, &altitude_sub1, &altitude_sub2, &altitude_timer, &altitude_state, 0, 0};

// Item pointer
Item* item;
