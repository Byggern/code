#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdio.h>
#include "CAN_driver.h"
#include "MCP2515_driver.h"
uint8_t test_data = 0b10100101;

const char canerror[] PROGMEM = "CAN interrupt due to error\n";
const char candefault[] PROGMEM = "CAN interrupt due to unhandled case\n";
const char canwake[] PROGMEM = "CAN interrupt due to being woken\n";
const char cannoint[] PROGMEM = "CAN interrupt with no IFLG code\n";
const char cantrans[] PROGMEM = "CAN interrupt due to transmission\n";
const char canthise[] PROGMEM = "This is EFLG and CANINTF: ";
const char canthisu[] PROGMEM = "This is INTF and ICOD: ";


bool message_received = false;
uint8_t intr_recv_buf[8] = {0,0,0,0,0,0,0,0};
CAN_MESSAGE CAN_receive_buf;//={.data=intr_recv_buf};



CAN_MESSAGE test_message = {
	.id = 0,
	.length = 1,
	.data = &test_data
};

void CAN_init(uint8_t id)
{
	CAN_receive_buf.data = intr_recv_buf;
	MCP_init();
	
	//enable interrupts
	sei();
	
	// Set interrupt pin as output
	INTERRUPT_DIR_PORT &= ~(1 << INTERRUPT_PIN);
	
#if defined(__AVR_ATmega162__)
	// Enable external interrupt on INT2
	GICR |=  (1 << INT2);
	// Interrupt on low signal
	EMCUCR &= ~(1 << ISC2);
	
#elif defined(__AVR_ATmega2560__)
	// Enable external interrupt on INT4
	EIMSK |= (1 << INT4);
	// Interrupt on low signal
	EICRB &= ~( (1 << ISC40) | (1 << ISC41));
#endif
	
	//Set Can interrupt bit for MSG Err, Wake Err, Error, Rx0, Rx1
	MCP_bit_modify(CANINTE, 7, 1);
	MCP_bit_modify(CANINTE, 6, 1);
	MCP_bit_modify(CANINTE, 5, 1);
	MCP_bit_modify(CANINTE, 1, 1);
	MCP_bit_modify(CANINTE, 0, 1);

	// Disable RTS pins
	MCP_write(BFPCTRL, 0);
	MCP_write(TXRTSCTRL, 0);
	
	//Receive buffer operation mode
	//01 = receive only valid messages with standard identifiers
	//11 = receive all messages
	MCP_bit_modify(RXBnCTRL + RXB0_OFFSET,5,1);
	MCP_bit_modify(RXBnCTRL + RXB0_OFFSET,6,1);
	MCP_bit_modify(RXBnCTRL + RXB1_OFFSET,5,1);
	MCP_bit_modify(RXBnCTRL + RXB1_OFFSET,6,1);
	
	MCP_write(RXBnSIDH + RXB0_OFFSET,(id >> 3));
	MCP_write(RXBnSIDL + RXB0_OFFSET,(id << 5));
	MCP_write(RXBnSIDH + RXB1_OFFSET,(id >> 3));
	MCP_write(RXBnSIDL + RXB1_OFFSET,(id << 5));
	
	MCP_bit_modify(CANCTRL, 5, 0);
	MCP_bit_modify(CANCTRL, 6, 0);
	MCP_bit_modify(CANCTRL, 7, 0);
	
	
}

void CAN_loopback_init(void)
{
	MCP_bit_modify(CANCTRL,5,0);
	MCP_bit_modify(CANCTRL,6,1);
	MCP_bit_modify(CANCTRL,7,0);
}

void CAN_send_message(uint8_t id, uint8_t buffer, CAN_MESSAGE * message) {
	uint8_t buf_offset = TXB0_OFFSET; // Buffer offset TODO: Make flexible buf_offset = buffer etc..
	
	// Check if TX buffer is ready 
	while(MCP_read(TXBnCTRL + buf_offset) & 0b1000)
	{
		//printf("TxW\n");
		// TXB currently pending transmission
	}
	
	// Set message ID to be transmitted 
	MCP_write(TXBnSIDH + buf_offset, (id >> 3));
	MCP_write(TXBnSIDL + buf_offset, (id << 5));
	// TXBnCTRL 1:0 = 0b11
	MCP_bit_modify(TXBnCTRL + buf_offset, 0, 1);
	MCP_bit_modify(TXBnCTRL + buf_offset, 1, 1);
	MCP_bit_modify(TXBnCTRL + buf_offset, 3, 1);
	MCP_write(TXBnDLC+buf_offset, message->length & 0b1111);
	
	// Write in data
	for (uint8_t i = 0; i < message->length; i++) {
		uint8_t data_reg = TXBnDm + buf_offset + i;
		//uint8_t data_reg = ((0b0011 + buffer) << 4) | (0b0110 + i);
		MCP_write(data_reg, message->data[i]);
	}
	
	MCP_request_to_send(buffer);
	
}


#if defined(__AVR_ATmega162__)
ISR(INT2_vect){
	int lambda = (MCP_read(0xe) & 0b1110)>>1;
	switch(lambda){
		case NOINT:{
			printf_P(cannoint);
			MCP_write(CANINTF, 0);
			break;
		}
		case ERROR:{
			printf_P(canerror);
			uint8_t eflg = MCP_read(EFLG);
			uint8_t iflg = MCP_read(CANINTF);
			printf_P(canthise);
			printf("%x, %x\n", eflg, iflg);
			MCP_bit_modify(CANINTF, 7, 0);
			break;
		}
		case WAKEUP:{
			printf_P(canwake);
			MCP_bit_modify(CANINTF, 6, 0);
			break;
		}
		case TX0: // fall through
		case TX1: // fall through
		case TX2:{
			printf_P(cantrans);
			MCP_bit_modify(CANINTF, 2, 0);
			MCP_bit_modify(CANINTF, 3, 0);
			MCP_bit_modify(CANINTF, 4, 0);
			break;
		}
		case RX0:{
			CAN_receive_message(RXBnDLC+RXB0_OFFSET, &CAN_receive_buf);
			MCP_bit_modify(CANINTF, 1, 0);
			break;
		}
		case RX1:{
			CAN_receive_message(RXBnDLC+RXB1_OFFSET, &CAN_receive_buf);
			MCP_bit_modify(CANINTF, 0, 0);
			break;
		}
		default:{
			printf_P(candefault);
			uint8_t iflg = MCP_read(CANINTF);
			printf_P(canthisu);
			printf("%x, %x\n", iflg, lambda);
			MCP_write(CANINTF, 0);
		}
	}
	
	// Clear interrupt flag just to be sure
	GIFR |= (1 << INTF2);
}
#elif defined(__AVR_ATmega2560__)
ISR(INT4_vect){
	int lambda = (MCP_read(0xe) & 0b1110)>>1;
	switch(lambda){
		case NOINT:{
			printf_P(cannoint);
			MCP_write(CANINTF, 0);
			break;
		}
		case ERROR:{
			printf_P(canerror);
			uint8_t eflg = MCP_read(EFLG);
			uint8_t iflg = MCP_read(CANINTF);
			printf_P(canthise);
			printf("%x, %x\n", eflg, iflg);
			MCP_bit_modify(CANINTF, 7, 0);
			break;
		}
		case WAKEUP:{
			printf_P(canwake);
			MCP_bit_modify(CANINTF, 6, 0);
			break;
		}
		case TX0: // fall through
		case TX1: // fall through
		case TX2:{
			printf_P(cantrans);
			MCP_bit_modify(CANINTF, 2, 0);
			MCP_bit_modify(CANINTF, 3, 0);
			MCP_bit_modify(CANINTF, 4, 0);
			break;
		}
		case RX0:{
			CAN_receive_message(RXBnDLC+RXB0_OFFSET, &CAN_receive_buf);
			MCP_bit_modify(CANINTF, 1, 0);
			break;
		}
		case RX1:{
			CAN_receive_message(RXBnDLC+RXB1_OFFSET, &CAN_receive_buf);
			MCP_bit_modify(CANINTF, 0, 0);
			break;
		}
		default:{
			printf_P(candefault);
			uint8_t iflg = MCP_read(CANINTF);
			printf_P(canthisu);
			printf("%x, %x\n", iflg, lambda);
			MCP_write(CANINTF, 0);
		}
	}
}
#endif



void CAN_receive_message( uint8_t messageaddr, CAN_MESSAGE * message)
{
	message->length = MCP_read(messageaddr) & 0b1111;
	
	for (int i = 0 ; i < message->length; i++)
	{
		message->data[i]=MCP_read(messageaddr + 1 + i);
	}
	message_received=true;
}