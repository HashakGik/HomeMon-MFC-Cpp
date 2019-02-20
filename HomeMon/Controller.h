#pragma once

#include "Controls/Control.h"
#include <vector>
#include <map>
#include <atlimage.h>
#include <string>

#include "Worker.h"

class Controller
{
public:
	Controller(CImage *img, std::string port, unsigned int baudRate);
	~Controller();
	void Attach(Control *c, std::vector<std::string> variables);
	void Detach(Control *c); // Detaches a control given its pointer.
	void Detach(std::string name); // Detaches a control given one of its attached variables.
	void Update(); // Fetches data from the worker and updates each control accordingly.
	void SaveScreenshot(std::string filename);
	void StartLogging(std::string filename);
	void StopLogging();
	void Stop();
	void Start(std::string port, unsigned int baudRate);
	void Serialize(std::string filename); // Serializes the internal state on a file.
	bool Deserialize(std::string filename); // Deserializes the internal state from a file.
	Control *GetControl(std::string name); // Finds a control from one of its attached variables.
	std::vector<Control *> GetControls();
	std::vector<std::string> GetVariables(Control *c);
	std::vector<std::string> GetControlNames();

	CImage *GetImg();
	void SetImg(CImage *img);
private:
	std::map<Control *, std::vector<std::string> > controls;
	std::map<std::string, int32_t> buffer;
	CImage *img;
	Worker w;
};

