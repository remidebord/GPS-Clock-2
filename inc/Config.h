#ifndef __CONFIG_H
#define __CONFIG_H

/* includes ------------------------------------------------------------------*/
#include "API.h" // DigigtalIn, I2C, Serial, ...

#include "SevenSeg.h"
#include "Encoder.h"
#include "DS3231.h"
#include "GPS.h"
#include "Button.h"
#include "Brightness.h"
#include "Timezone.h"
#include "Menu.h"

#include "Clock.h"
#include "Temperature.h"
#include "Altitude.h"
#include "Fix.h"
#include "Satellites.h"

/* defines --------------------------------------------------------------------*/
/* variables ------------------------------------------------------------------*/
extern Time time;
extern Date date;

/* objects --------------------------------------------------------------------*/
extern Ticker ticker;
extern DigitalOut segment[];
extern DigitalOut digit[];
extern DigitalOut dots;
extern SevenSeg sevenSeg0;
extern SevenSeg sevenSeg1;
extern SevenSeg sevenSeg2;
extern SevenSeg sevenSeg3;
extern SevenSegGroup sevenSegGroup;

extern InterruptIn encoderRightChA;
extern InterruptIn encoderRightChB;
extern InterruptIn encoderLeftChA;
extern InterruptIn encoderLeftChB;
extern Encoder encoderRight;
extern Encoder encoderLeft;	

extern Button buttonRight;
extern Button buttonLeft;
extern Brightness brightness;
extern Serial serial;
extern GNSS gnss;
extern I2C i2c;
extern DS3231 ds3231;
extern InterruptIn squareWave;
extern Timezone tz;
extern Menu menu;

extern Item clock;
extern Item temperature;
extern Item fix;
extern Item satellites;
extern Item altitude;
extern Item* item;

#endif /* __CONFIG_H */
