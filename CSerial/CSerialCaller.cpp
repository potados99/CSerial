#include "CSerialCaller.h"

CSerial * create_serial()
{
	return new CSerial();
}

void dispose_serial(CSerial *instance) {
	if (instance == NULL) { return; }

	delete instance;
	instance = NULL;
}

BOOL open(CSerial *instance, char * portname, int baudrate)
{
	if (instance == NULL) { return 0; }

	return instance->open(portname, baudrate);
}

BOOL close(CSerial *instance) {
	if (instance == NULL) { return 0; }

	return instance->close();
}

BOOL is_open(CSerial *instance) {
	if (instance == NULL) { return 0; }

	return instance->is_open();
}

size_t write(CSerial *instance, char *buffer, unsigned int size) {
	if (instance == NULL) { return 0; }

	return instance->write(buffer, size);
}

size_t read(CSerial *instance, char *buffer, unsigned int limit) {
	if (instance == NULL) { return 0; }

	return instance->read(buffer, limit);
}

DWORD bytes_to_read(CSerial *instance) {
	if (instance == NULL) { return 0; }

	return instance->bytes_to_read();
}