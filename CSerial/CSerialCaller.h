#pragma once

#include "stdafx.h"
#include "CSerial.h"

#ifdef __cplusplus
extern "C" {
#endif

	extern __declspec(dllexport) CSerial * create_serial();
	extern __declspec(dllexport) void dispose_serial(CSerial *);

	extern __declspec(dllexport) BOOL open(CSerial *, char *, int);
	extern __declspec(dllexport) BOOL close(CSerial *);
	extern __declspec(dllexport) BOOL is_open(CSerial *);

	extern __declspec(dllexport) size_t write(CSerial *, char *, unsigned int);
	extern __declspec(dllexport) size_t read(CSerial *, char *, unsigned int);

	extern __declspec(dllexport) DWORD bytes_to_read(CSerial *);

#ifdef __cplusplus
}
#endif
