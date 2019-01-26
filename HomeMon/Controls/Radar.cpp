#include "stdafx.h"
#include "Radar.h"

Radar::Radar(CImage *img, int cx, int cy, int radius, int rings, int size, fill_t fill, uint8_t fgR, uint8_t fgG, uint8_t fgB, uint8_t bgR, uint8_t bgG, uint8_t bgB) :
	Control(img, cx - radius, cy - radius, cx + radius, cy + radius, fgR, fgG, fgB, bgR, bgG, bgB)
{
	this->radius = radius;
	this->rings = rings;
	this->fill = fill;
	this->cx = cx;
	this->cy = cy;
	this->size = size;
	for (int i = 0; i < size; i++)
		this->oldVal.push_back(0);

	this->Init();
	this->type = C_RADAR;
}

Radar::Radar(CImage * img, std::map<std::string, int> state) :
	Control(img)
{
	this->SetState(state);
	this->type = C_RADAR;
}

void Radar::Update(std::vector<uint8_t> val)
{
	int i;
	std::vector<int> x(this->size), y(this->size);

	// If less than this->size values are passed, add zeroes.
	while (val.size() < this->size)
		val.push_back(0);

	for (i = 0; i < this->size; i++)
	{
		x[i] = this->oldVal[i] * this->radius / 0xff * cos(6.28 * i / this->size) + this->cx;
		y[i] = this->oldVal[i] * this->radius / 0xff * sin(6.28 * i / this->size) + this->cy;
	}

	if (this->fill == R_NOFILL)
		for (i = 0; i < this->size; i++)
			DrawLine(this->img, x[i], y[i], x[(i + 1) % this->size], y[(i + 1) % this->size], bgR, bgG, bgB);
	else
		FillPoly(this->img, this->cx - this->radius, this->cy - this->radius, this->cx + this->radius, this->cy + this->radius, x, y, this->size, bgR, bgG, bgB);

	for (i = 0; i < this->size; i++)
		DrawLine(this->img, this->cx, this->cy, this->radius * cos(6.28 / this->size * i) + this->cx, this->radius * sin(6.28 / this->size * i) + this->cy, this->fgR, this->fgG, this->fgB);

	for (i = 1; i <= this->rings; i++)
		DrawCircle(this->img, this->cx, this->cy, this->fgR, this->fgG, this->fgB, this->radius / this->rings * i);


	for (i = 0; i < this->size; i++)
	{
		x[i] = val[i] * this->radius / 0xff * cos(6.28 * i / this->size) + this->cx;
		y[i] = val[i] * this->radius / 0xff * sin(6.28 * i / this->size) + this->cy;
	}


	if (this->fill == R_NOFILL)
		for (i = 0; i < this->size; i++)
			DrawLine(this->img, x[i], y[i], x[(i + 1) % this->size], y[(i + 1) % this->size], fgR, fgG, fgB);
	else
		FillPoly(this->img, this->cx - this->radius, this->cy - this->radius, this->cx + this->radius, this->cy + this->radius, x, y, this->size, fgR, fgG, fgB);

	for (i = 0; i < this->size; i++)
		this->oldVal[i] = val[i];
}

void Radar::Init()
{
	int i;
	this->Control::Init();

	for (i = 0; i < this->size; i++)
		DrawLine(this->img, this->cx, this->cy, this->radius * cos(6.28 / this->size * i) + this->cx, this->radius * sin(6.28 / this->size * i) + this->cy, this->fgR, this->fgG, this->fgB);

	for (i = 1; i <= this->rings; i++)
		DrawCircle(this->img, this->cx, this->cy, this->fgR, this->fgG, this->fgB, this->radius / this->rings * i);
}

CImage *Radar::GetSample(RECT r)
{
	int i;
	std::vector<int> val(this->size), x(this->size), y(this->size);
	int radius = (((r.right - r.left) < (r.bottom - r.top)) ? (r.right - r.left) : (r.bottom - r.top)) / 2;
	int cx = (r.right - r.left) / 2;
	int cy = (r.bottom - r.top) / 2;

	if (this->sample == nullptr || this->sampleRect.left != r.left || this->sampleRect.right != r.right || this->sampleRect.top != r.top || this->sampleRect.bottom != r.bottom)
	{
		this->sample = Control::GetSample(r);

		for (i = 0; i < this->size; i++)
			DrawLine(this->sample, cx, cy, radius * cos(6.28 / this->size * i) + cx, radius * sin(6.28 / this->size * i) + cy, this->fgR, this->fgG, this->fgB);

		for (i = 1; i <= this->rings; i++)
			DrawCircle(this->sample, cx, cy, this->fgR, this->fgG, this->fgB, radius / this->rings * i);

		for (i = 0; i < this->size; i++)
		{
			val[i] = 91 * (i + 1) % 0x100;
			x[i] = val[i] * radius / 0xff * cos(6.28 * i / this->size) + cx;
			y[i] = val[i] * radius / 0xff * sin(6.28 * i / this->size) + cy;
		}

		if (this->fill == R_NOFILL)
			for (i = 0; i < this->size; i++)
				DrawLine(this->sample, x[i], y[i], x[(i + 1) % this->size], y[(i + 1) % this->size], fgR, fgG, fgB);
		else
			FillPoly(this->sample, 0, 0, 2 * radius, 2 * radius, x, y, this->size, fgR, fgG, fgB);
	}

	return this->sample;
}

std::map<std::string, int> Radar::GetState()
{
	std::map<std::string, int> memento = Control::GetState();

	memento["cx"] = this->cx;
	memento["cy"] = this->cy;
	memento["radius"] = this->radius;
	memento["rings"] = this->rings;
	memento["size"] = this->size;
	memento["fill"] = (this->fill == R_FILL) ? 0 : 1;

	return memento;
}

bool Radar::SetState(std::map<std::string, int> memento)
{
	bool ret;

	ret = memento.count("cx") == 1;
	ret &= memento.count("cy") == 1;
	ret &= memento.count("radius") == 1;
	ret &= memento.count("rings") == 1;
	ret &= memento.count("size") == 1;
	ret &= memento.count("fill") == 1;
	ret &= memento["radius"] > 0;
	ret &= memento["rings"] >= 0;
	ret &= memento["size"] > 0;
	ret &= memento["fill"] == 0 || memento["fill"] == 1;

	if (ret && Control::SetState(memento))
	{
		this->cx = memento["cx"];
		this->cy = memento["cy"];
		this->radius = memento["radius"];
		this->size = memento["size"];
		this->rings = memento["rings"];
		this->fill = (memento["fill"] == 0) ? R_FILL : R_NOFILL;

		// Correct this->rect in case there are inconsistencies with cx, cy and radius.
		this->rect.left = this->cx - this->radius;
		this->rect.right = this->cx + this->radius;
		this->rect.top = this->cy - this->radius;
		this->rect.bottom = this->cy + this->radius;

		this->oldVal.clear();
		for (int i = 0; i < this->size; i++)
			this->oldVal.push_back(0);
		this->Init();
		this->sample = nullptr;
	}

	return ret;
}