#pragma once

#include "Control.h"

class Accelerometer : public Control
{
public:
	typedef enum { A_SQUARE = 0, A_CIRCLE = 1 } shape_t;
	Accelerometer(CImage *img, int cx, int cy, int length, int rings, shape_t shp, int min, int max, uint8_t fgR, uint8_t fgG, uint8_t fgB, uint8_t bgR, uint8_t bgG, uint8_t bgB);
	Accelerometer(CImage *img, std::map<std::string, int> state);
	void Update(std::vector<int32_t> val);
	void Init();
	CImage *GetSample(RECT r);
	std::map<std::string, int> GetState();
	bool SetState(std::map<std::string, int> memento);
private:
	int cx;
	int cy;
	int length;
	int rings;
	shape_t shp;
};
