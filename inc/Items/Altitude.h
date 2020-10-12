#ifndef __ALTITUDE_H
#define __ALTITUDE_H

#include <stdint.h>

void altitude_init(void);
void altitude_main(void);
void altitude_sub1(void);
void altitude_sub2(void);
void altitude_timer(void);
uint8_t altitude_state(void);

#endif
