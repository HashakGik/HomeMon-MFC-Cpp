#pragma once

#include "Control.h"

class Plot : public Control
{
public:
	Plot(CImage *img, int x1, int y1, int x2, int y2, int min, int max, uint8_t fgR, uint8_t fgG, uint8_t fgB, uint8_t bgR, uint8_t bgG, uint8_t bgB);
	Plot(CImage *img, std::map<std::string, int> state);
	void Update(std::vector <int32_t> val);
	void Init();
	CImage *GetSample(RECT r);
	std::map<std::string, int> GetState();
	bool SetState(std::map<std::string, int> memento);
private:
	int i;
};