/*
 * timer.h
 *
 *  Created on: Dec 17, 2020
 *      Author: mrmah
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

void TIMER1_Deinit(void);
void TIMER1_init(uint16 time);

#endif /* TIMER_H_ */
