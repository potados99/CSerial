#pragma once

#include "stdafx.h"

class __declspec(dllexport) CSerial {
public:
	CSerial();
	~CSerial();

	BOOL open(char *, int);
	BOOL close();
	BOOL is_open();

	size_t write(char *, size_t);
	size_t read(char *, size_t);

	DWORD bytes_to_read();

protected:
	HANDLE  devHandle;
	OVERLAPPED overlappedRead, overlappedWrite;
	BOOL    opened;

private:
	void close_handles();
};