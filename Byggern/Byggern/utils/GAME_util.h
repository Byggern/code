/*
 * GAME_util.h
 *
 * Created: 20/11/2016 19:56:45
 *  Author: Magne
 */ 


#ifndef GAME_UTIL_H_
#define GAME_UTIL_H_

enum {
	GAME_RESTART = 0,
	GAME_CONTROLS,
	GAME_MISS
	} GAME_MESSAGE;

void GAME_send_controls(void);

void GAME_restart(void);

void GAME_check_messages(void);

#endif /* GAME_UTIL_H_ */