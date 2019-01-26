#pragma once

#include "Control.h"

class Dial : public Control
{
public:
	typedef enum { D_CLOCKWISE = 0, D_COUNTERCLOCKWISE = 1 } direction_t;
	Dial(CImage *img, int cx, int cy, double arc, double phase, int radius, int ticks, direction_t dir, uint8_t fgR, uint8_t fgG, uint8_t fgB, uint8_t bgR, uint8_t bgG, uint8_t bgB);
	Dial(CImage *img, std::map<std::string, int> state);
	void Update(std::vector <uint8_t> val);
	void Init();
	CImage *GetSample(RECT r);
	std::map<std::string, int> GetState();
	bool SetState(std::map<std::string, int> memento);
private:
	int radius;
	int cx;
	int cy;
	double arc;
	double phase;
	int ticks;
	direction_t dir;
};