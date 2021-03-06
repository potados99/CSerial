// CSerial.cpp: DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "CSerial.h"

CSerial::CSerial()
{
	opened = FALSE;
	devHandle = NULL;

	memset(&overlappedRead, 0, sizeof(OVERLAPPED));
	memset(&overlappedWrite, 0, sizeof(OVERLAPPED));
}

CSerial::~CSerial()
{
	close();
}

BOOL CSerial::open(char *portname, int baudrate)
{
	if (opened) { return true; }

	// make string
	wchar_t portName[25];
	StringCbPrintfW(portName, sizeof(portName), L"\\\\.\\%S", portname);

	// make handle
	devHandle = CreateFileW(
		portName, 
		GENERIC_READ | GENERIC_WRITE, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
		NULL
	);
	if (devHandle == NULL) { return false; }

	// apply timeouts
	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 5000;
	SetCommTimeouts(devHandle, &CommTimeOuts);

	// set overlapped struct
	overlappedRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	overlappedWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// get and set DCB
	DCB dcb;
	dcb.DCBlength = sizeof(DCB);
	GetCommState(devHandle, &dcb);

	dcb.BaudRate = baudrate;
	dcb.ByteSize = 8;

	if (SetCommState(devHandle, &dcb) == FALSE ||
		SetupComm(devHandle, 4096, 4096) == FALSE ||   //버퍼사이즈
		overlappedRead.hEvent == NULL || 
		overlappedWrite.hEvent == NULL) 
	{
		close_handles();

		return FALSE;
	}

	opened = TRUE;

	return TRUE;
}

BOOL CSerial::close()
{
	if (opened == FALSE || devHandle == NULL) { return true; }

	close_handles();

	opened = FALSE;
	devHandle = NULL;

	return TRUE;
}

BOOL CSerial::is_open() {
	return opened;
}

size_t CSerial::write(char * buffer, size_t size)
{
	if (! opened || devHandle == NULL) { return 0; }

	BOOL writeStatus;
	DWORD bytesWritten;

	writeStatus = WriteFile(devHandle, (LPSTR)buffer, size, &bytesWritten, &overlappedWrite);

	if (! writeStatus && (GetLastError() == ERROR_IO_PENDING)) {
		if (WaitForSingleObject(overlappedWrite.hEvent, 1000)) {
			bytesWritten = 0;
		}
		else {
			GetOverlappedResult(devHandle, &overlappedWrite, &bytesWritten, FALSE);
			overlappedWrite.Offset += bytesWritten;
		}
	}

	return (size_t)bytesWritten;
}

size_t CSerial::read(char * buffer, size_t limit)
{
	if (! opened || devHandle == NULL) { return 0; }

	BOOL readStatus;
	DWORD bytesRead;

	if ((bytesRead = bytes_to_read()) == 0) { /* return 0;*/ }
	if (limit < (size_t)bytesRead) { bytesRead = (DWORD)limit; }

	readStatus = ReadFile(devHandle, buffer, bytesRead, &bytesRead, &overlappedRead);

	if (! readStatus) {
		if (GetLastError() == ERROR_IO_PENDING) {
			WaitForSingleObject(overlappedRead.hEvent, 100);
			return (size_t)bytesRead;
		}

		return 0;
	}
	
	return (size_t)bytesRead;
}

DWORD CSerial::bytes_to_read() 
{
	if (! opened || devHandle == NULL) { return 0; }

	DWORD errorFlags;
	COMSTAT comStat;

	ClearCommError(devHandle, &errorFlags, &comStat);

	return comStat.cbInQue;
}

void CSerial::close_handles()
{
	if (overlappedRead.hEvent != NULL) { CloseHandle(overlappedRead.hEvent); }
	if (overlappedWrite.hEvent != NULL) { CloseHandle(overlappedWrite.hEvent); }
	if (devHandle != NULL) { CloseHandle(devHandle); }
}
