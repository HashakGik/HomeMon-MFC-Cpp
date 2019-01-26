#include "stdafx.h"
#include "Bar.h"

Bar::Bar(CImage *img, int x1, int y1, int x2, int y2, direction_t dir, uint8_t fgR, uint8_t fgG, uint8_t fgB, uint8_t bgR, uint8_t bgG, uint8_t bgB) :
	Control(img, x1, y1, x2, y2, fgR, fgG, fgB, bgR, bgG, bgB)
{
	this->dir = dir;
	this->oldVal.push_back(0);
	this->Init();
	this->type = C_BAR;
}

Bar::Bar(CImage * img, std::map<std::string, int> state) :
	Control(img)
{
	this->SetState(state);
	this->type = C_BAR;
}

void Bar::Update(std::vector<uint8_t> val)
{
	int i, j;
	int min = 0, max = 0;
	uint8_t r = bgR, g = bgG, b = bgB;

	if (this->dir == B_HORIZONTAL)
	{
		if (val[0] < this->oldVal[0])
		{
			min = val[0];
			max = this->oldVal[0];
			r = this->bgR;
			g = this->bgG;
			b = this->bgB;
		}
		else
		{
			max = val[0];
			min = this->oldVal[0];
			r = this->fgR;
			g = this->fgG;
			b = this->fgB;
		}

		for (i = this->rect.left + min * (this->rect.right - this->rect.left) / 0xff; i < this->rect.left + max * (this->rect.right - this->rect.left) / 0xff; i++)
			for (j = this->rect.top; j < this->rect.bottom; j++)
				this->SetSafePixelRGB(this->img, i, j, r, g, b);
	}
	else
	{
		if (val[0] < this->oldVal[0])
		{
			min = val[0];
			max = this->oldVal[0];
			r = this->bgR;
			g = this->bgG;
			b = this->bgB;
		}
		else
		{
			max = val[0];
			min = this->oldVal[0];
			r = this->fgR;
			g = this->fgG;
			b = this->fgB;
		}

		for (i = this->rect.left; i < this->rect.right; i++)
			for (j = this->rect.bottom - max * (this->rect.bottom - this->rect.top) / 0xff; j < this->rect.bottom - min * (this->rect.bottom - this->rect.top) / 0xff; j++)
				this->SetSafePixelRGB(this->img, i, j, r, g, b);
	}

	this->oldVal[0] = val[0];
}

void Bar::Init()
{
	this->Control::Init();
}

CImage *Bar::GetSample(RECT r)
{
	if (this->sample == nullptr || this->sampleRect.left != r.left || this->sampleRect.right != r.right || this->sampleRect.top != r.top || this->sampleRect.bottom != r.bottom)
	{
		this->sample = Control::GetSample(r);
		int i, j;

		if (this->dir == B_HORIZONTAL)
			for (i = 0; i < 127 * (r.right - r.left) / 0xff; i++)
				for (j = (r.bottom - r.top) / 2 - (r.bottom - r.top) / 10; j < (r.bottom - r.top) / 2 + (r.bottom - r.top) / 10; j++)
					this->SetSafePixelRGB(this->sample, i, j, this->fgR, this->fgG, this->fgB);
		else
			for (i = (r.right - r.left) / 2 - (r.right - r.left) / 10; i < (r.right - r.left) / 2 + (r.right - r.left) / 10; i++)
				for (j = r.bottom - r.top - 127 * (r.bottom - r.top) / 0xff; j < r.bottom - r.top; j++)
					this->SetSafePixelRGB(this->sample, i, j, this->fgR, this->fgG, this->fgB);
	}

	return this->sample;
}

std::map<std::string, int> Bar::GetState()
{
	std::map<std::string, int> memento = Control::GetState();

	memento["dir"] = (this->dir == B_HORIZONTAL) ? 0 : 1;

	return memento;
}

bool Bar::SetState(std::map<std::string, int> memento)
{
	bool ret;

	ret = memento.count("dir") == 1;
	ret &= memento["dir"] == 0 || memento["dir"] == 1;

	if (ret && Control::SetState(memento))
	{
		this->dir = (memento["dir"] == 0) ? B_HORIZONTAL : B_VERTICAL;
		this->oldVal.clear();
		this->oldVal.push_back(0);
		this->Init();
		this->sample = nullptr;
	}

	return ret;
}