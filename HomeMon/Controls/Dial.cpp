#include "stdafx.h"
#include "Dial.h"

Dial::Dial(CImage *img, int cx, int cy, double arc, double phase, int radius, int ticks, direction_t dir, int min, int max, uint8_t fgR, uint8_t fgG, uint8_t fgB, uint8_t bgR, uint8_t bgG, uint8_t bgB) :
	Control(img, cx - radius, cy - radius, cx + radius, cy + radius, min, max, fgR, fgG, fgB, bgR, bgG, bgB)
{
	this->cx = cx;
	this->cy = cy;
	this->radius = radius;
	this->ticks = ticks;
	this->arc = arc;
	this->phase = phase;
	this->dir = dir;
	this->oldVal.push_back(0);
	this->Init();
	this->type = C_DIAL;
}

Dial::Dial(CImage * img, std::map<std::string, int> state) :
	Control(img)
{
	this->SetState(state);
	this->type = C_DIAL;
}

void Dial::Update(std::vector<int32_t> val)
{
	uint8_t tmp;

	for (int i = 0; i < val.size(); i++)
	{
		val[i] -= this->min;
		if (val[i] < 0)
			val[i] = 0;
		else if (val[i] > this->max - this->min)
			val[i] = this->max - this->min;
	}

	tmp = (this->dir == D_COUNTERCLOCKWISE) ? 0xff - this->oldVal[0] : this->oldVal[0];
	DrawLine(this->img, this->cx, this->cy, this->radius * 0.8 * cos(this->phase + this->arc * tmp / (this->max - this->min)) + this->cx, this->radius * 0.8 * sin(this->phase + this->arc * tmp / (this->max - this->min)) + this->cy, this->bgR, this->bgG, this->bgB);
	tmp = (this->dir == D_COUNTERCLOCKWISE) ? 0xff - val[0] : val[0];
	DrawLine(this->img, this->cx, this->cy, this->radius * 0.8 * cos(this->phase + this->arc * tmp / (this->max - this->min)) + this->cx, this->radius * 0.8 * sin(this->phase + this->arc * tmp / (this->max - this->min)) + this->cy, this->fgR, this->fgG, this->fgB);

	this->oldVal[0] = val[0];
}

void Dial::Init()
{
	int i;
	double j, k;
	this->Control::Init();

	for (i = 0; i < this->arc * 200; i++)
	{
		j = this->radius * cos(i / 200.0 + this->phase) + this->cx;
		k = this->radius * sin(i / 200.0 + this->phase) + this->cy;

		this->SetSafePixelRGB(this->img, j, k, this->fgR, this->fgG, this->fgB);
	}

	int l;
	for (l = 0; l <= this->ticks; l++)
	{
		for (i = 0; i < this->radius * 0.15; i++)
		{
			j = this->cx + (this->radius - i) * cos(this->phase + l * this->arc / this->ticks);
			k = this->cy + (this->radius - i) * sin(this->phase + l * this->arc / this->ticks);
			this->SetSafePixelRGB(this->img, j, k, this->fgR, this->fgG, this->fgB);
		}
	}
}

CImage *Dial::GetSample(RECT r)
{
	int tmp, i;
	double j, k;
	int radius = (((r.right - r.left) < (r.bottom - r.top)) ? (r.right - r.left) : (r.bottom - r.top)) / 2;
	int cx = (r.right - r.left) / 2;
	int cy = (r.bottom - r.top) / 2;

	if (this->sample == nullptr || this->sampleRect.left != r.left || this->sampleRect.right != r.right || this->sampleRect.top != r.top || this->sampleRect.bottom != r.bottom)
	{
		this->sample = Control::GetSample(r);

		for (i = 0; i < this->arc * 200; i++)
		{
			j = radius * cos(i / 200.0 + this->phase) + cx;
			k = radius * sin(i / 200.0 + this->phase) + cy;

			this->SetSafePixelRGB(this->sample, j, k, this->fgR, this->fgG, this->fgB);
		}

		int l;
		for (l = 0; l <= this->ticks; l++)
		{
			for (i = 0; i < radius * 0.15; i++)
			{
				j = cx + (radius - i) * cos(this->phase + l * this->arc / this->ticks);
				k = cy + (radius - i) * sin(this->phase + l * this->arc / this->ticks);
				this->SetSafePixelRGB(this->sample, j, k, this->fgR, this->fgG, this->fgB);
			}
		}

		tmp = (this->dir == D_COUNTERCLOCKWISE) ? 0xff - 70 : 70;
		DrawLine(this->sample, cx, cy, radius * 0.8 * cos(this->phase + this->arc * tmp / 0xff) + cx, radius * 0.8 * sin(this->phase + this->arc * tmp / 0xff) + cy, this->fgR, this->fgG, this->fgB);
	}

	return this->sample;
}

std::map<std::string, int> Dial::GetState()
{
	std::map<std::string, int> memento = Control::GetState();

	memento["cx"] = this->cx;
	memento["cy"] = this->cy;
	memento["arc"] = this->arc * 10000;
	memento["phase"] = this->phase * 10000;
	memento["radius"] = this->radius;
	memento["ticks"] = this->ticks;
	memento["dir"] = (this->dir == D_CLOCKWISE) ? 0 : 1;

	return memento;
}

bool Dial::SetState(std::map<std::string, int> memento)
{
	bool ret;

	ret = memento.count("cx") == 1;
	ret &= memento.count("cy") == 1;
	ret &= memento.count("arc") == 1;
	ret &= memento.count("phase") == 1;
	ret &= memento.count("radius") == 1;
	ret &= memento.count("ticks") == 1;
	ret &= memento.count("dir") == 1;
	ret &= memento["radius"] > 0;
	ret &= memento["ticks"] >= 0;
	ret &= memento["dir"] == 0 || memento["dir"] == 1;

	if (ret && Control::SetState(memento))
	{
		this->cx = memento["cx"];
		this->cy = memento["cy"];
		this->arc = memento["arc"] / 10000.0;
		this->phase = memento["phase"] / 10000.0;
		this->radius = memento["radius"];
		this->ticks = memento["ticks"];
		this->dir = (memento["dir"] == 0) ? D_CLOCKWISE : D_COUNTERCLOCKWISE;

		// Correct this->rect in case there are inconsistencies with cx, cy and radius.
		this->rect.left = this->cx - this->radius;
		this->rect.right = this->cx + this->radius;
		this->rect.top = this->cy - this->radius;
		this->rect.bottom = this->cy + this->radius;

		this->oldVal.clear();
		this->oldVal.push_back(0);
		this->Init();
		this->sample = nullptr;
	}

	return ret;
}