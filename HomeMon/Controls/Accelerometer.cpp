#include "stdafx.h"
#include "Accelerometer.h"

Accelerometer::Accelerometer(CImage *img, int cx, int cy, int length, int rings, shape_t shp, int min, int max, uint8_t fgR, uint8_t fgG, uint8_t fgB, uint8_t bgR, uint8_t bgG, uint8_t bgB) :
	Control(img, cx - length / 2, cy - length / 2, cx + length / 2, cy + length / 2, min, max, fgR, fgG, fgB, bgR, bgG, bgB)
{
	this->length = length;
	this->rings = rings;
	this->shp = shp;
	this->cx = cx;
	this->cy = cy;
	this->Init();

	this->oldVal.push_back(0);
	this->oldVal.push_back(0);
	this->type = C_ACCELEROMETER;
}

Accelerometer::Accelerometer(CImage * img, std::map<std::string, int> state) :
	Control(img)
{
	this->SetState(state);
	this->type = C_ACCELEROMETER;
}

void Accelerometer::Update(std::vector<int32_t> val)
{
	int i, j, l;

	for (int i = 0; i < val.size(); i++)
	{
		val[i] -= this->min;
		if (val[i] < 0)
			val[i] = 0;
		else if (val[i] > this->max - this->min)
			val[i] = this->max - this->min;
	}

	// If less than two values are passed, add zeroes.
	while (val.size() < 2)
		val.push_back(0);

	for (i = -5; i < 5; i++)
	{
		if (this->cx - this->length / 2 + oldVal[0] * this->length / (this->max - this->min) + i >= this->rect.left && this->cx - this->length / 2 + oldVal[0] * this->length / (this->max - this->min) + i < this->rect.right &&
			this->cy - this->length / 2 + oldVal[1] * this->length / (this->max - this->min) >= this->rect.top && this->cy - this->length / 2 + oldVal[1] * this->length / (this->max - this->min) < this->rect.bottom)
			this->SetSafePixelRGB(this->img, this->cx - this->length / 2 + this->oldVal[0] * this->length / (this->max - this->min) + i, this->cy - this->length / 2 + this->oldVal[1] * this->length / (this->max - this->min), bgR, bgG, bgB);
		
		if (this->cx - this->length / 2 + oldVal[0] * this->length / (this->max - this->min) >= this->rect.left && this->img, this->cx - this->length / 2 + oldVal[0] * this->length / (this->max - this->min) < this->rect.right &&
			this->cy - this->length / 2 + oldVal[1] * this->length / (this->max - this->min) + i >= this->rect.top && this->cy - this->length / 2 + oldVal[1] * this->length / (this->max - this->min) + i < this->rect.bottom)
			this->SetSafePixelRGB(this->img, this->cx - this->length / 2 + this->oldVal[0] * this->length / (this->max - this->min), this->cy - this->length / 2 + this->oldVal[1] * this->length / (this->max - this->min) + i, bgR, bgG, bgB);
	}

	for (i = -5; i < 5; i++)
	{
		if (this->cx - this->length / 2 + val[0] * this->length / (this->max - this->min) + i >= this->rect.left && this->cx - this->length / 2 + val[0] * this->length / (this->max - this->min) + i < this->rect.right &&
			this->cy - this->length / 2 + val[1] * this->length / (this->max - this->min) >= this->rect.top && this->cy - this->length / 2 + val[1] * this->length / (this->max - this->min) < this->rect.bottom)
			this->SetSafePixelRGB(this->img, this->cx - this->length / 2 + val[0] * this->length / (this->max - this->min) + i, this->cy - this->length / 2 + val[1] * this->length / (this->max - this->min), fgR, fgG, fgB);
		
		if (this->cx - this->length / 2 + val[0] * this->length / (this->max - this->min) >= this->rect.left && this->img, this->cx - this->length / 2 + val[0] * this->length / (this->max - this->min) < this->rect.right &&
			this->cy - this->length / 2 + val[1] * this->length / (this->max - this->min) + i >= this->rect.top && this->cy - this->length / 2 + val[1] * this->length / (this->max - this->min) + i < this->rect.bottom)
			this->SetSafePixelRGB(this->img, this->cx - this->length / 2 + val[0] * this->length / (this->max - this->min), this->cy - this->length / 2 + val[1] * this->length / (this->max - this->min) + i, fgR, fgG, fgB);
	}

	if (this->shp == A_SQUARE)
		for (j = 1; j <= this->rings; j++)
			for (i = -this->length / this->rings / 2 * j; i <= this->length / this->rings / 2 * j; i++)
			{
				this->SetSafePixelRGB(this->img, this->cx + i, this->cy - this->length / this->rings / 2 * j, fgR, fgG, fgB);
				this->SetSafePixelRGB(this->img, this->cx + i, this->cy + this->length / this->rings / 2 * j, fgR, fgG, fgB);
				this->SetSafePixelRGB(this->img, this->cx - this->length / this->rings / 2 * j, this->cy + i, fgR, fgG, fgB);
				this->SetSafePixelRGB(this->img, this->cx + this->length / this->rings / 2 * j, this->cy + i, fgR, fgG, fgB);
			}
	else
		for (l = 1; l <= this->rings; l++)
			DrawCircle(this->img, this->cx, this->cy, this->fgR, this->fgG, this->fgB, this->length / 2 / this->rings * l);

	this->oldVal[0] = val[0];
	this->oldVal[1] = val[1];
}

void Accelerometer::Init()
{
	this->Control::Init();

	int i, j, l;

	if (this->shp == A_SQUARE)
		for (j = 1; j <= this->rings; j++)
			for (i = -this->length / this->rings / 2 * j; i <= this->length / this->rings / 2 * j; i++)
			{
				this->SetSafePixelRGB(this->img, this->cx + i, this->cy - this->length / this->rings / 2 * j, this->fgR, this->fgG, this->fgB);
				this->SetSafePixelRGB(this->img, this->cx + i, this->cy + this->length / this->rings / 2 * j, this->fgR, this->fgG, this->fgB);
				this->SetSafePixelRGB(this->img, this->cx - this->length / this->rings / 2 * j, this->cy + i, this->fgR, this->fgG, this->fgB);
				this->SetSafePixelRGB(this->img, this->cx + this->length / this->rings / 2 * j, this->cy + i, this->fgR, this->fgG, this->fgB);
			}
	else
		for (l = 1; l <= this->rings; l++)
			DrawCircle(this->img, this->cx, this->cy, this->fgR, this->fgG, this->fgB, this->length / 2 / this->rings * l);
}

CImage *Accelerometer::GetSample(RECT r)
{
	if (this->sample == nullptr || this->sampleRect.left != r.left || this->sampleRect.right != r.right || this->sampleRect.top != r.top || this->sampleRect.bottom != r.bottom)
	{
		this->sample = Control::GetSample(r);

		int i, j, l;
		int length = ((r.right - r.left) < (r.bottom - r.top)) ? (r.right - r.left) : (r.bottom - r.top);
		int x = (r.right - r.left) / 2;
		int y = (r.bottom - r.top) / 2;

		if (this->shp == A_SQUARE)
			for (j = 1; j <= this->rings; j++)
				for (i = -length / this->rings / 2 * j; i <= length / this->rings / 2 * j; i++)
				{
					this->SetSafePixelRGB(this->sample, x + i, y - length / this->rings / 2 * j, fgR, fgG, fgB);
					this->SetSafePixelRGB(this->sample, x + i, y + length / this->rings / 2 * j, fgR, fgG, fgB);
					this->SetSafePixelRGB(this->sample, x - length / this->rings / 2 * j, y + i, fgR, fgG, fgB);
					this->SetSafePixelRGB(this->sample, x + length / this->rings / 2 * j, y+ i, fgR, fgG, fgB);
				}
		else
			for (l = 1; l <= this->rings; l++)
				DrawCircle(this->sample, x, y, this->fgR, this->fgG, this->fgB, length / 2 / this->rings * l);


		for (i = -5; i < 5; i++)
		{
			this->SetSafePixelRGB(this->sample, x + 20 * length / 0xff + i, y - 60 * length / 0xff, this->fgR, this->fgG, this->fgB);
			this->SetSafePixelRGB(this->sample, x + 20 * length / 0xff, y - 60 * length / 0xff + i, this->fgR, this->fgG, this->fgB);
		}
	}

	return this->sample;
}

std::map<std::string, int> Accelerometer::GetState()
{
	std::map<std::string, int> memento = Control::GetState();

	memento["cx"] = this->cx;
	memento["cy"] = this->cy;
	memento["length"] = this->length;
	memento["rings"] = this->rings;
	memento["shp"] = (this->shp == A_SQUARE) ? 0 : 1;

	return memento;
}

bool Accelerometer::SetState(std::map<std::string, int> memento)
{
	bool ret;

	ret = memento.count("cx") == 1;
	ret &= memento.count("cy") == 1;
	ret &= memento.count("length") == 1;
	ret &= memento.count("rings") == 1;
	ret &= memento.count("shp") == 1;
	ret &= memento["length"] > 0;
	ret &= memento["rings"] >= 0;
	ret &= memento["shp"] == 0 || memento["shp"] == 1;

	if (ret && Control::SetState(memento))
	{
		this->cx = memento["cx"];
		this->cy = memento["cy"];
		this->length = memento["length"];
		this->rings = memento["rings"];
		this->shp = (memento["shp"] == 0) ? A_SQUARE : A_CIRCLE;

		// Correct this->rect in case there are inconsistencies with cx, cy and length.
		this->rect.left = this->cx - this->length / 2;
		this->rect.right = this->cx + this->length / 2;
		this->rect.top = this->cy - this->length / 2;
		this->rect.bottom = this->cy + this->length / 2;

		this->oldVal.clear();
		this->oldVal.push_back(0);
		this->oldVal.push_back(0);
		this->Init();
		this->sample = nullptr;
	}

	return ret;
}