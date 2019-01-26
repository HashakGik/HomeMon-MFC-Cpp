#pragma once

#include <string>
#include <map>
#include <mutex>
#include <atomic>
#include <thread>
#include <iostream>
#include <fstream>

#include "Utils/Serial.h"

class Worker
{
public:
	Worker();
	~Worker();
	void Start(std::string port, unsigned int baudRate);
	void StartLogging(std::string filename);
	std::map<std::string, uint8_t> GetValues();
	void Stop();
	void StopLogging();
private:
	std::map<std::string, uint8_t> values;
	std::mutex lock;
	std::atomic<bool> quit;
	std::thread *thd;
	Serial serial;
	std::atomic<bool> log;
	std::ofstream file;
	void Run();
	//void Write(std::string str);
	std::string ReadLine();
};

