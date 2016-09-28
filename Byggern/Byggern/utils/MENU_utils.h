#pragma once
#include <stdint.h>
#ifndef MENU_UTILS_H_
#define MENU_UTILS_H_

typedef struct Menu_t {
	char * header;
	void (* function)(void);
	struct Menu_t * parent;
	struct Menu_t * submenus;
	uint8_t length;
	uint8_t position;
} Menu;

#endif /* MENU_UTILS_H_ */