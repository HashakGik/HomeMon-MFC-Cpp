#pragma once

#include <atlimage.h>
#include <string>
#include <map>
#include <vector>

// Control base class.
class Control
{
public:
	typedef enum { C_DIAL, C_PLOT, C_BAR, C_ACCELEROMETER, C_RADAR } control_t;
	~Control();
	virtual void Update(std::vector<int32_t> val) = 0;
	virtual CImage *GetSample(RECT r); // Modified Singleton pattern. If the rectangle does not change, returns a stored sample.
	virtual void Init();
	void SetImg(CImage *img);
	CImage *GetImg();
	virtual std::map<std::string, int> GetState(); // Memento pattern.
	virtual bool SetState(std::map<std::string, int> memento);
	control_t GetType();
protected:
	Control(CImage *img, int x1, int y1, int x2, int y2, int min, int max, uint8_t fgR, uint8_t fgG, uint8_t fgB, uint8_t bgR, uint8_t bgG, uint8_t bgB);
	Control(CImage *img);
	void DrawLine(CImage *img, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b);
	void FillPoly(CImage *img, int x0, int y0, int x1, int y1, std::vector<int> x, std::vector<int> y, int n, uint8_t r, uint8_t g, uint8_t b);
	void DrawCircle(CImage *img, int x0, int y0, uint8_t r, uint8_t g, uint8_t b, int radius);
	void SetSafePixelRGB(CImage *img, int x, int y, uint8_t r, uint8_t g, uint8_t b);
	CImage *img, *sample = nullptr;
	RECT rect;
	RECT sampleRect;
	int min, max;
	std::vector<int32_t> oldVal;
	uint8_t fgR, fgG, fgB, bgR, bgG, bgB;
	control_t type;
};