#ifndef __SATELLITES_H
#define __SATELLITES_H

#include <stdint.h>

void satellites_init(void);
void satellites_main(void);
void satellites_sub1(void);
void satellites_sub2(void);
void satellites_timer(void);
uint8_t satellites_state(void);

#endif
