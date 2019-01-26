#pragma once

#include <string>

// Settings storage class. It decouples the other classes from the specific implementation.
class Settings
{
public:
	static void Set(std::string filename, std::string section, std::string key, std::string value);
	static void Set(std::string filename, std::string section, std::string key, CString value);
	static void Set(std::string filename, std::string section, std::string key, int value);
	static std::string GetString(std::string filename, std::string section, std::string key, std::string defaultValue = "");
	static int GetInt(std::string filename, std::string section, std::string key, int defaultValue = 0);
};
