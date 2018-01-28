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

void InterpolationEngine::ChangeSolution(char * outputFileName, int multiplicator)
{
	createEmptyBitmap(multiplicator);
	fillExistingPixels(multiplicator);
	fillHorizontalGaps(multiplicator);
	fillVerticalGaps(multiplicator);
	ins_crebm(&bitmapOutput, outputFileName);
}

void InterpolationEngine::createEmptyBitmap(int multiplicator)
{
	int pow = multiplicator*multiplicator;
	bitmapOutput.iHeight = bitmapInput.iHeight * multiplicator;
	bitmapOutput.iWidth = bitmapInput.iWidth * multiplicator;
	bitmapOutput.sFillByteCount = (bitmapOutput.iWidth % 4);
	bitmapOutput.iDatenlaenge = bitmapInput.iHeight * bitmapInput.iWidth * (pow * 3) + bitmapOutput.sFillByteCount * bitmapInput.iHeight;
	bitmapOutput.iDateilaenge = 54 + bitmapOutput.iDatenlaenge;
	bitmapOutput.byData = new BYTE[bitmapOutput.iDatenlaenge];
	bitmapOutput.byHeader = (BYTE*)malloc(54);
	for (size_t i = 0; i < 54; i++)
	{
		bitmapOutput.byHeader[i] = bitmapInput.byHeader[i];
	}
	ins_overwritehead(&bitmapOutput);
}

void InterpolationEngine::fillExistingPixels(int multiplicator)
{
	unsigned int iCurrentOutputIndex = 0;
	unsigned int iCurrentInputIndex = 0;
	unsigned int iLineLength = bitmapOutput.iWidth * 3 + bitmapOutput.sFillByteCount;
	unsigned int iStepX = multiplicator * 3;
	unsigned int iStepY = (multiplicator-1) * iLineLength;
	for (size_t i = 0; i < bitmapInput.iHeight; i++)
	{
		for (size_t j = 0; j < bitmapInput.iWidth; j++)
		{
			if (r)
				bitmapOutput.byData[iCurrentOutputIndex] = bitmapInput.byData[iCurrentInputIndex];
			if (g)
				bitmapOutput.byData[iCurrentOutputIndex + 1] = bitmapInput.byData[iCurrentInputIndex + 1];
			if (b)
				bitmapOutput.byData[iCurrentOutputIndex + 2] = bitmapInput.byData[iCurrentInputIndex + 2];
			
			iCurrentOutputIndex += iStepX;
			iCurrentInputIndex += 3;
		}
		iCurrentOutputIndex += iStepY;
		iCurrentInputIndex += bitmapOutput.sFillByteCount;
	}
}

void InterpolationEngine::fillHorizontalGaps(int multiplicator)
{
	unsigned int iCurrentOutputIndex = 3;
	unsigned int iLineLength = bitmapOutput.iWidth * 3 + bitmapOutput.sFillByteCount;
	unsigned int iStepX = multiplicator * 3;
	unsigned int iStepX2 = (multiplicator - 1) * 3;
	unsigned int iStepY = (multiplicator - 1) * iLineLength;
	for (size_t i = 0; i < bitmapInput.iHeight; i++)
	{
		for (size_t j = 0; j < bitmapInput.iWidth - 1; j++)
		{
			for (size_t k = 0; k < 3; k++)
			{
				int a = (bitmapOutput.byData[iCurrentOutputIndex + k + iStepX-3] - bitmapOutput.byData[iCurrentOutputIndex + k - 3])/multiplicator;
				int b = bitmapOutput.byData[iCurrentOutputIndex + k - 3];
				for (size_t x = 0; x < multiplicator; x++)
				{
					bitmapOutput.byData[iCurrentOutputIndex + k + x * 3] = a*x+b;
				}
			}
			iCurrentOutputIndex += iStepX;
		}
		for (size_t j = 0; j < iStepX; j++)
		{
			bitmapOutput.byData[iCurrentOutputIndex + j] = bitmapOutput.byData[iCurrentOutputIndex + j - 3];
		}
		iCurrentOutputIndex += iStepX;
		for (size_t j = 0; j < bitmapInput.sFillByteCount; j++)
		{
			bitmapOutput.byData[iCurrentOutputIndex + j] = 0;
		}
		iCurrentOutputIndex += iStepY;
	}
}

void InterpolationEngine::fillVerticalGaps(int multiplicator)
{
	unsigned int iLineLength = bitmapOutput.iWidth * 3 + bitmapOutput.sFillByteCount;
	unsigned int iCurrentOutputIndex = 0;
	unsigned int iCurrentUpperLineIndex = 0;
	unsigned int iCurrentLowerLineIndex = multiplicator * iLineLength;
	unsigned int iStepX = multiplicator * 3;
	unsigned int iShiftY = (multiplicator-1) * iLineLength;
	for (size_t i = 0; i < bitmapInput.iHeight - 1; i++)
	{
		for (size_t j = 0; j < bitmapOutput.iWidth; j++)
		{
			for (size_t k = 0; k < 3; k++)
			{
				int a = (bitmapOutput.byData[iCurrentLowerLineIndex + k] - bitmapOutput.byData[iCurrentUpperLineIndex + k]) / multiplicator;
				int b = bitmapOutput.byData[iCurrentUpperLineIndex + k];
				for (size_t x = 0; x < multiplicator; x++)
				{
					bitmapOutput.byData[iCurrentOutputIndex + k + x * iLineLength] = a*x + b;
				}
			}
			iCurrentOutputIndex += 3;
			iCurrentUpperLineIndex += 3;
			iCurrentLowerLineIndex += 3;
		}
		for (size_t k = 0; k < bitmapInput.sFillByteCount; k++)
		{
			bitmapOutput.byData[iCurrentOutputIndex + k] = 0;
		}
		iCurrentOutputIndex += iShiftY;
		iCurrentUpperLineIndex += iShiftY;
		iCurrentLowerLineIndex += iShiftY;
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
