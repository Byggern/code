#ifndef CAN_DRIVER_H_
#define CAN_DRIVER_H_
#include <stdbool.h>
//Note that extern is a declaration of a variable without definition

typedef struct CAN_MESSAGE_T {
	uint8_t id;
	uint8_t length;
	uint8_t * data;
	} CAN_MESSAGE;

extern CAN_MESSAGE CAN_receive_buf;

extern bool message_received;
void CAN_init(void);

void CAN_loopback_init(void);

void CAN_send_message(uint8_t id, uint8_t buffer, CAN_MESSAGE * message);


void CAN_receive_message( uint8_t buffer, CAN_MESSAGE * message);


#endif /* CAN_DRIVER_H_ */