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
	InterpolationEngine(stBitmap bitmap);
	~InterpolationEngine();
	void DoubleSolution(char* outputFileName);

private:
	stBitmap bitmapOutput;
	stBitmap bitmapInput;

	void createEmptyBitmap();
	void fillExistingPixels();
	void fillHorizontalGaps();
	void fillVerticalGaps();
	BYTE calcByte(BYTE a, BYTE b);
};
