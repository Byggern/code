

#ifndef CAN_DRIVER_H_
#define CAN_DRIVER_H_

typedef struct CAN_MESSAGE_T {
	uint8_t id;
	uint8_t length;
	uint8_t * data;
	} CAN_MESSAGE;

void CAN_send_message(uint8_t id, CAN_MESSAGE * message);

void CAN_receive_message(uint8_t id, CAN_MESSAGE * message);


#endif /* CAN_DRIVER_H_ */