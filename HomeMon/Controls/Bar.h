#pragma once

#include "Control.h"

class Bar : public Control
{
public:
	typedef enum { B_HORIZONTAL = 0, B_VERTICAL = 1 } direction_t;
	Bar(CImage *img, int x1, int y1, int x2, int y2, direction_t dir, uint8_t fgR, uint8_t fgG, uint8_t fgB, uint8_t bgR, uint8_t bgG, uint8_t bgB);
	Bar(CImage *img, std::map<std::string, int> state);
	void Update(std::vector <uint8_t> val);
	void Init();
	CImage *GetSample(RECT r);
	std::map<std::string, int> GetState();
	bool SetState(std::map<std::string, int> memento);
private:
	direction_t dir;
};