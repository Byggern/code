#pragma once
#include <stdint.h>
#ifndef MENU_UTILS_H_
#define MENU_UTILS_H_

typedef struct Menu_t {
	const char * header;
	void (* function)(void);
	struct Menu_t *submenus[8];
	uint8_t length;
} Menu;

Menu menus[];

void MENU_link_menus(void);

void MENU_redraw( Menu * menu, uint8_t position);

void MENU( Menu * menu);


#endif /* MENU_UTILS_H_ */