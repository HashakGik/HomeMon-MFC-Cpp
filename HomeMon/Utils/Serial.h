#pragma once

#include <string>
#include <vector>

// Serial communication class. It decouples the other classes from the specific implementation.
class Serial
{
public:
	Serial();
	~Serial();
	bool Open(std::string port, int baud);
	bool Open();
	bool Close();
	//bool Write(std::string str);
	char ReadChar();
	std::string ReadLine();
	std::vector<std::string> GetAvailablePorts();
private:
	std::string port;
	int baud;
	HANDLE hCom;
	DCB dcb;
	COMMTIMEOUTS timeout;
};

