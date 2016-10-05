
uint8_t test_data = 0b10100101;

CAN_MESSAGE test_message = {
	.id = 0,
	.length = 1,
	.data = &test_data;
}

void CAN_init(void) {
	
}

void CAN_send_message(uint8_t buffer, CAN_MESSAGE * message) {
	
	for (uint8_t i = 0; i < message->length; i++) {
		uint8_t data_reg = ((0b0011 + buffer) << 4) | (0b0110 + i);
		MCP_write(data_reg, message->data[i]);
	}
	
	MCP_request_to_send(buffer);
	
}

void CAN_receive_message(CAN_MESSAGE * message) {
	
}