/*
 * GAME2_util.h
 *
 * Created: 20/11/2016 21:13:39
 *  Author: Magne
 */ 


#ifndef GAME2_UTIL_H_
#define GAME2_UTIL_H_

#include <stdbool.h>

volatile extern bool can_timeslot_free;

void GAME2_init(void);

bool GAME2_timeout(void);

void GAME2_check_messages(void);

void GAME2_check_sensors(void);

void GAME2_send_miss(void);

void GAME2_update_regulator(void);

#endif /* GAME2_UTIL_H_ */