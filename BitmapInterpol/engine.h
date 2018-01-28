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
	void ChangeSolution(char* outputFileName, int multiplicator);

private:
	stBitmap bitmapOutput;
	stBitmap bitmapInput;

	void createEmptyBitmap();
	void createEmptyBitmap(int multiplicator);
	void fillExistingPixels();
	void fillHorizontalGaps();
	void fillExistingPixels(int multiplicator);
	void fillHorizontalGaps(int multiplicator);
	void fillVerticalGaps();
	void fillVerticalGaps(int multiplicator);
	BYTE calcByte(BYTE a, BYTE b);
};
