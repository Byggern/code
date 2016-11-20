#ifndef CAN_DRIVER_H_
#define CAN_DRIVER_H_
#include <stdbool.h>

#define CANINTE 0x2b
/* Set up pins corresponding to boards */
#if defined(__AVR_ATmega162__)

#define INTERRUPT_DIR_PORT DDRE
#define INTERRUPT_PIN PE0
#define CAN_ID 0

#elif defined(__AVR_ATmega2560__)

#define INTERRUPT_DIR_PORT DDRE
#define INTERRUPT_PIN PE4
#define CAN_ID 1

#endif

typedef enum CAN_INTERRUPT_tag {
	NOINT	= 0b000,
	ERROR	= 0b001,
	WAKEUP	= 0b010,
	TX0		= 0b011,
	TX1		= 0b100,
	TX2		= 0b101,
	RX0		= 0b110,
	RX1		= 0b111
} CAN_INTERRUPT;

typedef struct CAN_MESSAGE_T {
	uint8_t id;
	uint8_t length;
	uint8_t  *data;
	} CAN_MESSAGE;
	
//Note that extern is a declaration of a variable without definition
volatile extern CAN_MESSAGE CAN_receive_buf;

extern bool message_received;
void CAN_init(uint8_t id, uint8_t loopback);

void CAN_loopback_init(void);

void CAN_send_message(uint8_t id, CAN_MESSAGE * message);

void CAN_receive_message( uint8_t buffer, CAN_MESSAGE * message);


#endif /* CAN_DRIVER_H_ */