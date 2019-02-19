#include "stdafx.h"
#include "Control.h"

#include <algorithm>
#include <cmath>

Control::Control(CImage *img, int x1, int y1, int x2, int y2, uint8_t fgR, uint8_t fgG, uint8_t fgB, uint8_t bgR, uint8_t bgG, uint8_t bgB)
{
	this->img = img;
	this->rect.top = y1;
	this->rect.bottom = y2;
	this->rect.left = x1;
	this->rect.right = x2;
	this->fgR = fgR;
	this->fgG = fgG;
	this->fgB = fgB;
	this->bgR = bgR;
	this->bgG = bgG;
	this->bgB = bgB;
	this->sampleRect.top = this->sampleRect.bottom = this->sampleRect.left = this->sampleRect.right = 0;
}

Control::Control(CImage * img)
{
	this->img = img;
	this->sampleRect.top = this->sampleRect.bottom = this->sampleRect.left = this->sampleRect.right = 0;
}

Control::~Control()
{
	int i, j;

	for (i = this->rect.top; i <= this->rect.bottom; i++)
		for (j = this->rect.left; j <= this->rect.right; j++)
			this->SetSafePixelRGB(this->img, j, i, this->bgR, this->bgG, this->bgB);

	if (this->sample != nullptr)
		delete this->sample;
}

CImage * Control::GetSample(RECT r)
{
	CImage *ret = new CImage();

	this->sampleRect.right = r.right;
	this->sampleRect.left = r.left;
	this->sampleRect.top = r.top;
	this->sampleRect.bottom = r.bottom;

	ret->Create(r.right - r.left, r.bottom - r.top, 24);

	for (int i = 0; i < r.right - r.left; i++)
		for (int j = 0; j < r.bottom - r.top; j++)
			this->SetSafePixelRGB(ret, i, j, this->bgR, this->bgG, this->bgB);

	return ret;
}

void Control::Init()
{
	for (int i = this->rect.left; i < this->rect.right; i++)
		for (int j = this->rect.top; j < this->rect.bottom; j++)
			this->SetSafePixelRGB(this->img, i, j, this->bgR, this->bgG, this->bgB);
}

void Control::SetImg(CImage * img)
{
	this->img = img;
}

CImage * Control::GetImg()
{
	return this->img;
}

std::map<std::string, int> Control::GetState()
{
	std::map<std::string, int> memento;

	memento["x0"] = this->rect.left;
	memento["x1"] = this->rect.right;
	memento["y0"] = this->rect.top;
	memento["y1"] = this->rect.bottom;
	memento["bgr"] = this->bgR;
	memento["bgg"] = this->bgG;
	memento["bgb"] = this->bgB;
	memento["fgr"] = this->fgR;
	memento["fgg"] = this->fgG;
	memento["fgb"] = this->fgB;

	return memento;
}

bool Control::SetState(std::map<std::string, int> memento)
{
	bool ret;

	// Check if every filed is defined and then validate them.
	ret = memento.count("x0") == 1;
	ret &= memento.count("x1") == 1;
	ret &= memento.count("y0") == 1;
	ret &= memento.count("y1") == 1;
	ret &= memento.count("bgr") == 1;
	ret &= memento.count("bgg") == 1;
	ret &= memento.count("bgb") == 1;
	ret &= memento.count("fgr") == 1;
	ret &= memento.count("fgg") == 1;
	ret &= memento.count("fgb") == 1;
	ret &= memento["x0"] < memento["x1"];
	ret &= memento["y0"] < memento["y1"];
	ret &= memento["bgr"] >= 0 && memento["bgr"] <= 0xff;
	ret &= memento["bgg"] >= 0 && memento["bgg"] <= 0xff;
	ret &= memento["bgb"] >= 0 && memento["bgb"] <= 0xff;
	ret &= memento["fgr"] >= 0 && memento["fgr"] <= 0xff;
	ret &= memento["fgg"] >= 0 && memento["fgg"] <= 0xff;
	ret &= memento["fgb"] >= 0 && memento["fgb"] <= 0xff;

	// If there are no errors, restore the state.
	if (ret)
	{
		this->rect.left = memento["x0"];
		this->rect.right = memento["x1"];
		this->rect.top = memento["y0"];
		this->rect.bottom = memento["y1"];
		this->fgR = memento["fgr"];
		this->fgG = memento["fgg"];
		this->fgB = memento["fgb"];
		this->bgR = memento["bgr"];
		this->bgG = memento["bgg"];
		this->bgB = memento["bgb"];

		this->Control::Init();
	}

	return ret;
}

Control::control_t Control::GetType()
{
	return this->type;
}


void Control::DrawCircle(CImage *img, int x0, int y0, uint8_t r, uint8_t g, uint8_t b, int radius)
{
	int x = radius - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (radius << 1);

	while (x >= y)
	{
		this->SetSafePixelRGB(img, x0 + x, y0 + y, r, g, b);
		this->SetSafePixelRGB(img, x0 + y, y0 + x, r, g, b);
		this->SetSafePixelRGB(img, x0 - y, y0 + x, r, g, b);
		this->SetSafePixelRGB(img, x0 - x, y0 + y, r, g, b);
		this->SetSafePixelRGB(img, x0 - x, y0 - y, r, g, b);
		this->SetSafePixelRGB(img, x0 - y, y0 - x, r, g, b);
		this->SetSafePixelRGB(img, x0 + y, y0 - x, r, g, b);
		this->SetSafePixelRGB(img, x0 + x, y0 - y, r, g, b);

		if (err <= 0)
		{
			y++;
			err += dy;
			dy += 2;
		}
		if (err > 0)
		{
			x--;
			dx += 2;
			err += dx - (radius << 1);
		}
	}
}

void Control::SetSafePixelRGB(CImage * img, int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
	if (x >= 0 && y >= 0 && x < img->GetWidth() && y < img->GetHeight())
		img->SetPixelRGB(x, y, r, g, b);
}

void Control::DrawLine(CImage *img, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b)
{
	int dx, dy, err, yi, xi, x, y;
	if ((y1 - y0) * (y1 - y0) < (x1 - x0) * (x1 - x0))
	{
		if (x0 > x1)
		{
			dx = x0 - x1;
			dy = y0 - y1;
			yi = 1;
			if (dy < 0)
			{
				yi = -1;
				dy = -dy;
			}
			err = 2 * dy - dx;
			y = y1;
			for (x = x1; x < x0; x++)
			{
				this->SetSafePixelRGB(img, x, y, r, g, b);
				if (err > 0)
				{
					y += yi;
					err -= 2 * dx;
				}
				err += 2 * dy;
			}
		}
		else
		{
			dx = x1 - x0;
			dy = y1 - y0;
			yi = 1;
			if (dy < 0)
			{
				yi = -1;
				dy = -dy;
			}
			err = 2 * dy - dx;
			y = y0;
			for (x = x0; x < x1; x++)
			{
				this->SetSafePixelRGB(img, x, y, r, g, b);
				if (err > 0)
				{
					y += yi;
					err -= 2 * dx;
				}
				err += 2 * dy;
			}
		}
	}
	else
	{
		if (y0 > y1)
		{
			dy = y0 - y1;
			dx = x0 - x1;
			xi = 1;
			if (dx < 0)
			{
				xi = -1;
				dx = -dx;
			}
			err = 2 * dx - dy;
			x = x1;
			for (y = y1; y < y0; y++)
			{
				this->SetSafePixelRGB(img, x, y, r, g, b);
				if (err > 0)
				{
					x += xi;
					err -= 2 * dy;
				}
				err += 2 * dx;
			}
		}
		else
		{
			dy = y1 - y0;
			dx = x1 - x0;
			xi = 1;
			if (dx < 0)
			{
				xi = -1;
				dx = -dx;
			}
			err = 2 * dx - dy;
			x = x0;
			for (y = y0; y < y1; y++)
			{
				this->SetSafePixelRGB(img, x, y, r, g, b);
				if (err > 0)
				{
					x += xi;
					err -= 2 * dy;
				}
				err += 2 * dx;
			}
		}
	}
}

void Control::FillPoly(CImage *img, int x0, int y0, int x1, int y1, std::vector<int> x, std::vector<int> y, int n, uint8_t r, uint8_t g, uint8_t b)
{
	int i, j, l;
	std::vector<int> intercepts;

	for (i = y0; i < y1; i++)
	{
		intercepts.clear();
		j = n - 1;
		for (l = 0; l < n; l++)
		{
			if ((y[l] < i && y[j] >= i) ||
				(y[j] < i && y[l] >= i))
				intercepts.push_back(x[l] + 1.0 * (i - 1.0 * y[l]) / (1.0 * y[j] - y[l]) * (1.0 * x[j] - x[l]));
			j = l;
		}

		std::sort(intercepts.begin(), intercepts.end());

		for (j = 0; j < intercepts.size() && intercepts[j] < x1; j += 2)
		{
			if (intercepts[j + 1] > x0)
			{
				if (intercepts[j] < x0)
					intercepts[j] = x0;
				if (intercepts[j + 1] > x1)
					intercepts[j + 1] = x1;

				for (l = intercepts[j]; l < intercepts[j + 1]; l++)
					this->SetSafePixelRGB(img, l, i, r, g, b);
			}
		}
	}
}