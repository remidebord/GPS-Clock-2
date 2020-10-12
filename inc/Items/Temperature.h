#ifndef __TEMPERATURE_H
#define __TEMPERATURE_H

#include <stdint.h>

void temperature_init(void);
void temperature_main(void);
void temperature_sub1(void);
void temperature_sub2(void);
void temperature_timer(void);
uint8_t temperature_state(void);

#endif
