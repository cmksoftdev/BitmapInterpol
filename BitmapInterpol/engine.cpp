#include "engine.h"


InterpolationEngine::InterpolationEngine(stBitmap bitmap)
{
	if (&bitmap == NULL)
		throw ERROR_DATA;
	bitmapInput = bitmap;
	bitmapOutput.byData = NULL;
	bitmapOutput.byHeader = NULL;
	bitmapOutput.byHeaderExp = NULL;
}

InterpolationEngine::~InterpolationEngine()
{
	if (bitmapOutput.byData != NULL)
		free(bitmapOutput.byData);
	if (bitmapOutput.byHeader != NULL)
		free(bitmapOutput.byHeader);
	if (bitmapOutput.byHeaderExp != NULL)
		free(bitmapOutput.byHeaderExp);
}

void InterpolationEngine::DoubleSolution(char * outputFileName)
{
	createEmptyBitmap();
	fillExistingPixels();
	fillHorizontalGaps();
	fillVerticalGaps();
	ins_crebm(&bitmapOutput, outputFileName);
}

//In the first test we will double the solution.
void InterpolationEngine::createEmptyBitmap()
{
	bitmapOutput.iHeight = bitmapInput.iHeight * 2;
	bitmapOutput.iWidth = bitmapInput.iWidth * 2;
	bitmapOutput.sFillByteCount = (bitmapOutput.iWidth % 4);
	bitmapOutput.iDatenlaenge = bitmapInput.iHeight * bitmapInput.iWidth * 12 + bitmapOutput.sFillByteCount * bitmapInput.iHeight;
	bitmapOutput.iDateilaenge = 54 + bitmapOutput.iDatenlaenge;
	bitmapOutput.byData = new BYTE[bitmapOutput.iDatenlaenge];
	bitmapOutput.byHeader = (BYTE*)malloc(54);
	for (size_t i = 0; i < 54; i++)
	{
		bitmapOutput.byHeader[i] = bitmapInput.byHeader[i];
	}
	ins_overwritehead(&bitmapOutput);
}

void InterpolationEngine::fillExistingPixels()
{
	unsigned int iCurrentOutputIndex = 0;
	unsigned int iCurrentInputIndex = 0;
	unsigned int iLineLength = bitmapOutput.iWidth*3 + bitmapOutput.sFillByteCount;
	for (size_t i = 0; i < bitmapInput.iHeight; i++)
	{
		for (size_t j = 0; j < bitmapInput.iWidth; j++)
		{
			for (size_t k = 0; k < 3; k++)
			{
				bitmapOutput.byData[iCurrentOutputIndex + k] = bitmapInput.byData[iCurrentInputIndex + k];
			}
			iCurrentOutputIndex += 6;
			iCurrentInputIndex += 3;
		}
		iCurrentOutputIndex += iLineLength;
		iCurrentInputIndex += bitmapOutput.sFillByteCount;
	}
}

void InterpolationEngine::fillHorizontalGaps()
{
	unsigned int iCurrentOutputIndex = 3;
	for (size_t i = 0; i < bitmapOutput.iHeight; i++)
	{		
		for (size_t j = 0; j < bitmapInput.iWidth - 1; j++)
		{
			for (size_t k = 0; k < 3; k++)
			{
				bitmapOutput.byData[iCurrentOutputIndex + k] = calcByte(bitmapOutput.byData[iCurrentOutputIndex + k - 3],
					bitmapOutput.byData[iCurrentOutputIndex + k + 3]);
			}
			iCurrentOutputIndex += 6;
		}
		for (size_t j = 0; j < 3; j++)
		{
			bitmapOutput.byData[iCurrentOutputIndex + j] = bitmapOutput.byData[iCurrentOutputIndex + j - 3];
		}
		iCurrentOutputIndex += 6;
		for (size_t j = 0; j < bitmapInput.sFillByteCount; j++)
		{
			bitmapOutput.byData[iCurrentOutputIndex + j] = 0;
		}
		iCurrentOutputIndex += bitmapInput.sFillByteCount;
	}
}

void InterpolationEngine::fillVerticalGaps()
{
	unsigned int iLineLength = bitmapOutput.iWidth * 3 + bitmapOutput.sFillByteCount;
	unsigned int iCurrentOutputIndex = iLineLength;
	unsigned int iCurrentUpperLineIndex = 0;
	unsigned int iCurrentLowerLineIndex = 2 * iLineLength;
	for (size_t i = 0; i < bitmapInput.iHeight - 1; i++)
	{
		for (size_t j = 0; j < bitmapOutput.iWidth; j++)
		{
			for (size_t k = 0; k < 3; k++)
			{
				bitmapOutput.byData[iCurrentOutputIndex + k] = calcByte(bitmapOutput.byData[iCurrentUpperLineIndex + k],
					bitmapOutput.byData[iCurrentLowerLineIndex + k]);
			}
			iCurrentOutputIndex += 3;
			iCurrentUpperLineIndex += 3;
			iCurrentLowerLineIndex += 3;
		}
		for (size_t k = 0; k < bitmapInput.sFillByteCount; k++)
		{
			bitmapOutput.byData[iCurrentOutputIndex + k] = 0;
		}
		iCurrentOutputIndex += iLineLength;
		iCurrentUpperLineIndex += iLineLength;
		iCurrentLowerLineIndex += iLineLength;
	}
	for (size_t i = 0; i < bitmapOutput.iWidth * 3; i++)
	{
		bitmapOutput.byData[iCurrentOutputIndex + i] = bitmapOutput.byData[iCurrentUpperLineIndex + i];
	}
	for (size_t i = 0; i < bitmapInput.sFillByteCount; i++)
	{
		bitmapOutput.byData[iCurrentOutputIndex + i] = 0;
	}
}

BYTE InterpolationEngine::calcByte(BYTE a, BYTE b)
{
	return (a + b) / 2;
}
