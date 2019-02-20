#include "stdafx.h"
#include "Worker.h"

#include <ctime>
#include <cstdio>


Worker::Worker() :
	serial()
{
	this->quit = false;
	this->log = false;
}


Worker::~Worker()
{
	this->serial.Close();
}

void Worker::Start(std::string port, unsigned int baudRate)
{
	if (this->serial.Open(port, baudRate))
	{
		this->quit = false;
		this->thd = new std::thread(&Worker::Run, this);
	}
	else
		this->thd = nullptr;
}

void Worker::StartLogging(std::string filename)
{
	this->lock.lock();
	this->file.open(filename);
	this->lock.unlock();
	this->log = true;
}

void Worker::Run()
{
	std::string param, line;
	int32_t val;
	time_t now;
	char timestamp[20]; // 19 chars, plus \0.

	while (!this->quit)
	{
		try
		{
			line = this->ReadLine();

			if (line != "")
			{
				if (this->log)
				{
					this->file << "[";
					time(&now);
					strftime(timestamp, 20, "%F %T", localtime(&now));
					this->file << timestamp;
					this->file << "] ";
					this->file << line;
					this->file << "\r\n";
				}

				param = line.substr(0, line.find(" "));
				val = (int) std::stol(line.substr(line.find(" "), line.length()));
				this->lock.lock();
				this->values[param] = val;
				this->lock.unlock();
			}
		}
		catch (std::invalid_argument)	
		{
			// Exception in stol. Since the flow is interrupted before updating the values, there is no need to take any action.
		}
		catch (std::out_of_range)
		{
			// Exception in stol. Since the flow is interrupted before updating the values, there is no need to take any action.
		}
	}

	if (this->log)
		this->StopLogging();
}

/*
void Worker::Write(std::string str)
{
	this->serial.write(str);
}
*/

std::string Worker::ReadLine()
{
	return this->serial.ReadLine();
}

std::map<std::string, int32_t> Worker::GetValues()
{
	std::map<std::string, int32_t> ret;
	this->lock.lock();
	ret = this->values;
	this->lock.unlock();

	return ret;
}

void Worker::Stop()
{
	try
	{
		this->quit = true;
		if (this->thd != nullptr && this->thd->joinable())
			this->thd->join();
		this->serial.Close();
	}
	catch (std::system_error)
	{
		// Exception in join. Since the thread needs to be closed anyway, there is no need to take any action.
	}
}

void Worker::StopLogging()
{
	this->log = false;
	this->lock.lock();
	this->file.close();
	this->lock.unlock();
}
