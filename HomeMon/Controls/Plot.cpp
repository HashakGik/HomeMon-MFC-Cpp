#include "stdafx.h"
#include "Plot.h"

Plot::Plot(CImage * img, int x1, int y1, int x2, int y2, uint8_t fgR, uint8_t fgG, uint8_t fgB, uint8_t bgR, uint8_t bgG, uint8_t bgB) :
	Control(img, x1, y1, x2, y2, fgR, fgG, fgB, bgR, bgG, bgB)
{
	this->i = 0;
	this->oldVal.push_back(0);
	this->type = C_PLOT;
}

Plot::Plot(CImage * img, std::map<std::string, int> state) :
	Control(img)
{
	this->SetState(state);
	this->i = 0;
	this->type = C_PLOT;
}

void Plot::Update(std::vector<uint8_t> val)
{
	int j;

	for (j = this->rect.top; j < this->rect.bottom; j++)
		if (j > this->rect.bottom - (this->rect.bottom - this->rect.top) * val[0] / 0xff)
			this->SetSafePixelRGB(this->img, this->i + this->rect.left, j, this->fgR, this->fgG, this->fgB);
		else
			this->SetSafePixelRGB(this->img, this->i + this->rect.left, j, this->bgR, this->bgG, this->bgB);
	this->i = (this->i + 1) % (this->rect.right - this->rect.left);
}

void Plot::Init()
{
	this->Control::Init();
	this->i = 0;
}

CImage *Plot::GetSample(RECT r)
{
	int i, j;
	if (this->sample == nullptr || this->sampleRect.left != r.left || this->sampleRect.right != r.right || this->sampleRect.top != r.top || this->sampleRect.bottom != r.bottom)
	{
		this->sample = Control::GetSample(r);

		for (i = 0; i < 300; i++)
			for (j = 0; j < r.bottom - r.top; j++)
				if (j > r.bottom - r.top - (r.bottom - r.top) * (cos(i * 6.28 / 50.0 + 2.7)*sin(i * 6.28 / 100.0) / 2 + 0.5))
					this->SetSafePixelRGB(this->sample, i % (r.right - r.left), j, this->fgR, this->fgG, this->fgB);
	}

	return this->sample;
}

std::map<std::string, int> Plot::GetState()
{
	return Control::GetState();
}

bool Plot::SetState(std::map<std::string, int> memento)
{
	bool ret = Control::SetState(memento);
	if (ret)
	{
		this->oldVal.clear();
		this->oldVal.push_back(0);
		this->Init();
		this->sample = nullptr;
	}

	return ret;
}