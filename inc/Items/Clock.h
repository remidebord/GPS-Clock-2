#ifndef __CLOCK_H
#define __CLOCK_H

#include <stdint.h>

#define CLOCK_SYNC_TIME 1000 // 1000ms

void clock_init(void);
void clock_main(void);
void clock_sub1(void);
void clock_sub2(void);
void clock_timer(void);
uint8_t clock_state(void);

#endif
