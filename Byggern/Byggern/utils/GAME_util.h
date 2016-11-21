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
	GAME_MISS,
	GAME_CALIBRATE_BOARD,
	GAME_STOP,
	} GAME_MESSAGE;

void GAME_send_controls(void);

uint8_t GAME_check_miss(void);

void GAME_draw_status(uint8_t health, uint16_t time_alive);

void GAME_func(void);

void GAME_stop(void);

void GAME_restart(void);

void GAME_calibrate_board(void);

#endif /* GAME_UTIL_H_ */