#include "stdafx.h"
#include "Serial.h"


Serial::Serial()
{
	FillMemory(&(this->dcb), sizeof(this->dcb), 0);
	FillMemory(&(this->timeout), sizeof(this->timeout), 0);
}


Serial::~Serial()
{

}

bool Serial::Open(std::string port, int baud)
{
	this->port = port;
	this->baud = baud;

	return this->Open();
}

bool Serial::Open()
{
	bool ret = true;
	wchar_t p[128];

	mbstowcs(p, this->port.c_str(), 128);

	this->hCom = CreateFile(p, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	ret &= (this->hCom != INVALID_HANDLE_VALUE);

	if (ret)
		ret &= GetCommState(this->hCom, &(this->dcb));
	if (ret)
	{
		this->dcb.BaudRate = this->baud;
		this->dcb.ByteSize = 8;
		this->dcb.Parity = NOPARITY;
		this->dcb.StopBits = ONESTOPBIT;

		ret &= SetCommState(this->hCom, &(this->dcb));
	}
	if (ret)
	{
		this->timeout.ReadIntervalTimeout = 1000;
		this->timeout.ReadTotalTimeoutConstant = 1000;
		this->timeout.ReadTotalTimeoutMultiplier = 1;
		this->timeout.WriteTotalTimeoutConstant = 1000;
		this->timeout.WriteTotalTimeoutMultiplier = 1;

		ret &= (SetCommTimeouts(this->hCom, &(this->timeout)));
	}

	return ret;
}

bool Serial::Close()
{
	bool ret = CloseHandle(this->hCom);
	this->hCom = INVALID_HANDLE_VALUE;

	return ret;
}

/*
bool Serial::Write(std::string str) // false on error.
{
	bool ret = true;



	return ret;
}
*/

char Serial::ReadChar() // \0 on error.
{
	char ret = '\0';
	DWORD incoming;

	if (this->hCom != INVALID_HANDLE_VALUE)
		if (!ReadFile(this->hCom, &ret, 1, &incoming, NULL))
			this->Close();

	return ret;
}

std::string Serial::ReadLine()
{
	std::string ret;
	char c;

	do
	{
		c = this->ReadChar();
		if (c != '\0' && c != '\n' && c != '\r')
			ret += c;

	} while (c != '\0' && c != '\n' && c != '\r');

	// If ReadChar returns \0 (an error occurred), drop the entire line.
	if (c == '\0')
		ret = "";

	return ret;
}

std::vector<std::string> Serial::GetAvailablePorts()
{
	std::vector<std::string> ret;

	for (int i = 1; i < 256; i++)
		if (this->Open("\\\\.\\COM" + std::to_string(i), 9600))
		{
			this->Close();
			ret.push_back("\\\\.\\COM" + std::to_string(i));
		}

	return ret;
}
