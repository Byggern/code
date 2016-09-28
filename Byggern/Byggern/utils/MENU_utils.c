#include "MENU_utils.h"

/* struct Menu {
char * header;
void (* function)(void);
struct Menu * parent;
struct Menu * submenus;
uint8_t length;
uint8_t position;
};*/

static struct Menu * current;


void MENU( Menu * menu){
	if ( menu->function){
		function();
	}
	MENU_draw();
	MENU_select();
	
}