#include "stdafx.h"
#include "Controller.h"
#include "Controls/Controls.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>


Controller::Controller(CImage *img, std::string port, unsigned int baudRate)
{
	this->img = img;
	this->w.Start(port, baudRate);
}


Controller::~Controller()
{
	this->w.Stop();
}

void Controller::Attach(Control * c, std::vector<std::string> variables)
{
	this->controls.insert_or_assign(c, variables);

	c->SetImg(this->img);
	c->Init();
	this->Update();
}

void Controller::Detach(Control * c)
{
	this->controls.erase(c);
	delete c;
}

void Controller::Detach(std::string name)
{
	Control *c = this->GetControl(name);
	this->Detach(c);
}

void Controller::Update()
{
	std::vector<uint8_t> tmp;

	this->buffer = this->w.GetValues();

	for (std::pair<Control *, std::vector<std::string> > p : this->controls)
	{
		tmp.clear();
		for (std::string s : p.second)
		{
			if (this->buffer.find(s) != this->buffer.end())
				tmp.push_back(this->buffer[s]);
			else
				tmp.push_back(0);
		}
		if (tmp.size() > 0)
			p.first->Update(tmp);
	}
}

CImage *Controller::GetImg()
{
	return this->img;
}

void Controller::SetImg(CImage *img)
{
	this->img->Destroy();
	delete this->img;
	this->img = img;
	for (std::pair<Control *, std::vector<std::string> > p : this->controls)
	{
		p.first->SetImg(img);
		p.first->Init();
	}
}

void Controller::SaveScreenshot(std::string filename)
{
	wchar_t *tmp = new wchar_t[filename.length() + 1];
	mbstowcs(tmp, filename.c_str(), filename.length());
	tmp[filename.length()] = '\0';

	this->img->Save(tmp, GUID_NULL);

	delete tmp;
}

void Controller::StartLogging(std::string filename)
{
	this->w.StartLogging(filename);
}

void Controller::StopLogging()
{
	this->w.StopLogging();
}

void Controller::Stop()
{
	this->w.Stop();
}

void Controller::Start(std::string port, unsigned int baudRate)
{
	this->w.Start(port, baudRate);
}

void Controller::Serialize(std::string filename)
{
	std::ofstream file;
	std::map<std::string, int> tmp;
	int k, l;

	file.open(filename);

	k = 0;
	for (std::pair<Control *, std::vector<std::string> > i : this->controls)
	{
		switch (i.first->GetType())
		{
		case Control::C_DIAL:
			file << "[" << k << "_dial]\r\n";
			break;
		case Control::C_PLOT:
			file << "[" << k << "_plot]\r\n";
			break;
		case Control::C_BAR:
			file << "[" << k << "_bar]\r\n";
			break;
		case Control::C_ACCELEROMETER:
			file << "[" << k << "_accelerometer]\r\n";
			break;
		case Control::C_RADAR:
			file << "[" << k << "_radar]\r\n";
			break;
		default:
			file << "[" << k << "_unknown]\r\n";
			break;
		}

		// Save the control's state.
		tmp = i.first->GetState();
		for (std::pair<std::string, int> j : tmp)
			file << j.first << "=" << j.second << "\r\n";

		// Append the control's variables.
		l = 0;
		if (i.second.size() > 0)
			file << "variables=";
		for (std::string j : i.second)
		{
			file << j;
			if (l < i.second.size() - 1)
				file << ";";
			else
				file << "\r\n";
			l++;
		}

		k++;
	}
	file.close();
}

bool Controller::Deserialize(std::string filename)
{
	std::ifstream file;
	std::string line, controlName, tmp;
	std::map<std::string, std::map<std::string, int> > data;
	std::vector<std::string> vars;
	std::stringstream s;
	bool ret = true;
	Control *c;

	file.open(filename);
	if (!file.fail())
	{
		do
		{
			std::getline(file, line);

			if (line.length() != 0)
			{
				// Strip the final \r (getline ignores it).
				if (line.at(line.length() - 1) == '\r')
					line.pop_back();
				if (line.at(0) == '[' && line.at(line.length() - 1) == ']')
				{
					controlName = line.substr(1, line.length() - 2);
					data[controlName] = std::map<std::string, int>();
				}
				else if (line.find("variables=") != std::string::npos)
				{
					s = std::stringstream(line.substr(line.find("=") + 1));
					vars.clear();
					while (std::getline(s, tmp, ';'))
						vars.push_back(tmp);

					if (controlName.find("dial") != std::string::npos)
						c = new Dial(this->img, data[controlName]);
					else if (controlName.find("plot") != std::string::npos)
						c = new Plot(this->img, data[controlName]);
					else if (controlName.find("bar") != std::string::npos)
						c = new Bar(this->img, data[controlName]);
					else if (controlName.find("accelerometer") != std::string::npos)
						c = new Accelerometer(this->img, data[controlName]);
					else if (controlName.find("radar") != std::string::npos)
						c = new Radar(this->img, data[controlName]);

					this->Attach(c, vars);
				}
				else
					data[controlName][line.substr(0, line.find("="))] = std::stoi(line.substr(line.find("=") + 1, line.size()));
			}
		} while (!file.eof());

		file.close();
	}
	else
		ret = false;
	
	return ret;
}

Control * Controller::GetControl(std::string name)
{
	for (std::pair<Control *, std::vector<std::string> > i : this->controls)
		for (std::string s : i.second)
			if (s == name)
				return i.first;
}

std::vector<Control *> Controller::GetControls()
{
	std::vector<Control *> ret;

	for (std::pair<Control *, std::vector<std::string> > p : this->controls)
		ret.push_back(p.first);

	return ret;
}

std::vector<std::string> Controller::GetVariables(Control * c)
{
	return this->controls[c];
}

std::vector<std::string> Controller::GetControlNames()
{
	std::vector<std::string> ret;
	char tmp[256];
	int i = 0;

	for (std::pair<Control *, std::vector<std::string> > p : this->controls)
	{
		switch (p.first->GetType())
		{
		case Control::C_DIAL:
			sprintf(tmp, "%d_dial", i);
			break;
		case Control::C_PLOT:
			sprintf(tmp, "%d_plot", i);
			break;
		case Control::C_BAR:
			sprintf(tmp, "%d_bar", i);
			break;
		case Control::C_ACCELEROMETER:
			sprintf(tmp, "%d_accelerometer", i);
			break;
		case Control::C_RADAR:
			sprintf(tmp, "%d_radar", i);
			break;
		default:
			sprintf(tmp, "%d_unknown", i);
			break;
		}
		ret.push_back(std::string(tmp));

		i++;
	}

	return ret;
}

