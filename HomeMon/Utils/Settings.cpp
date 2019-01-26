#include "stdafx.h"

#include "Settings.h"


void Settings::Set(std::string filename, std::string section, std::string key, std::string value)
{
	wchar_t file[1024], sec[128], k[128], val[128];
	
	mbstowcs(file, filename.c_str(), 1024);
	mbstowcs(sec, section.c_str(), 128);
	mbstowcs(k, key.c_str(), 128);
	mbstowcs(val, value.c_str(), 128);

	WritePrivateProfileString(sec, k, val, file);
}

void Settings::Set(std::string filename, std::string section, std::string key, CString value)
{
	char val[128];

	wcstombs(val, value.GetBuffer(), 128);
	Settings::Set(filename, section, key, val);
}

void Settings::Set(std::string filename, std::string section, std::string key, int value)
{
	wchar_t file[1024], sec[128], k[128], val[128];

	mbstowcs(file, filename.c_str(), 1024);
	mbstowcs(sec, section.c_str(), 128);
	mbstowcs(k, key.c_str(), 128);

	_itow(value, val, 10);

	WritePrivateProfileString(sec, k, val, file);
}

std::string Settings::GetString(std::string filename, std::string section, std::string key, std::string defaultValue)
{
	wchar_t file[1024], sec[128], k[128], val[128], def[128];
	char ret[128];

	mbstowcs(file, filename.c_str(), 1024);
	mbstowcs(sec, section.c_str(), 128);
	mbstowcs(k, key.c_str(), 128);
	mbstowcs(def, defaultValue.c_str(), 128);
	GetPrivateProfileString(sec, k, def, val, 128, file);

	wcstombs(ret, val, 128);

	return std::string(ret);
}

int Settings::GetInt(std::string filename, std::string section, std::string key, int defaultValue)
{
	wchar_t file[1024], sec[128], k[128];

	mbstowcs(file, filename.c_str(), 1024);
	mbstowcs(sec, section.c_str(), 128);
	mbstowcs(k, key.c_str(), 128);

	return GetPrivateProfileInt(sec, k, defaultValue, file);
}
