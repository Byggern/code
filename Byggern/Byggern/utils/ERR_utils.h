typedef struct {
	char function[10];
	char code[10];
	char data[44];
} error;

void throwError(error error);