#pragma once

#include "Control.h"

class Radar : public Control
{
public:
	typedef enum { R_FILL = 0, R_NOFILL = 1 } fill_t;
	Radar(CImage *img, int cx, int cy, int radius, int rings, int size, fill_t fill, uint8_t fgR, uint8_t fgG, uint8_t fgB, uint8_t bgR, uint8_t bgG, uint8_t bgB);
	Radar(CImage *img, std::map<std::string, int> state);
	void Update(std::vector<uint8_t> val);
	void Init();
	CImage *GetSample(RECT r);
	std::map<std::string, int> GetState();
	bool SetState(std::map<std::string, int> memento);
private:
	int cx;
	int cy;
	int radius;
	int rings;
	int size;
	fill_t fill;
};
