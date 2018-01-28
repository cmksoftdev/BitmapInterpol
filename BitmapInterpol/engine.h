#pragma once
#include "bitmap.h"

struct RGB
{
	BYTE r;
	BYTE g;
	BYTE b;
};

struct RGBA
{
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE a;
};

class InterpolationEngine
{
public:
	bool r = true, g = false, b = true;
	InterpolationEngine(stBitmap bitmap);
	~InterpolationEngine();
	void ChangeSolution(char* outputFileName, int multiplicator);

private:
	stBitmap bitmapOutput;
	stBitmap bitmapInput;

	void createEmptyBitmap(int multiplicator);
	void fillExistingPixels(int multiplicator);
	void fillHorizontalGaps(int multiplicator);
	void fillVerticalGaps(int multiplicator);
};
