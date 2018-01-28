#include "bitmap.h"

//TODO Refactor to cpp
stBitmap* ins_chopbm(char* dat)
{
	FILE *datei;
	stBitmap *zeiger;
	BYTE byMagischeZeichen[2];
	long lPixelstart, lDateilaenge;

	if ((zeiger = (stBitmap*)malloc(sizeof(stBitmap))) == NULL)
	{
		return ERROR_MEMO;
	}
	fopen_s(&datei, dat, "rb");
	if (datei == NULL)
	{
		free(zeiger);
		return ERROR_DAT;
	}
	else
	{
		fseek(datei, SEEK_SET, SEEK_END);
		zeiger->iDateilaenge = ftell(datei);
		rewind(datei);
		if (zeiger->iDateilaenge > 54)
		{
			byMagischeZeichen[0] = fgetc(datei);
			byMagischeZeichen[1] = fgetc(datei);
			if (byMagischeZeichen[0] == 'B'&&byMagischeZeichen[1] == 'M')
			{
				rewind(datei);
				if ((zeiger->byHeader = (BYTE*)malloc(54)) == NULL)
				{
					free(zeiger);
					fclose(datei);
					return  ERROR_MEMO;
				}
				else
				{
					zeiger->iHeaderlaenge = 54;
					if (fread(zeiger->byHeader, 1, zeiger->iHeaderlaenge, datei) != zeiger->iHeaderlaenge)
					{
						free(zeiger);
						fclose(datei);
						return ERROR_DAT;
					}
					lDateilaenge = zeiger->byHeader[2];
					lDateilaenge = lDateilaenge + zeiger->byHeader[3] * 256;
					lDateilaenge = lDateilaenge + zeiger->byHeader[4] * 65536;
					lDateilaenge = lDateilaenge + zeiger->byHeader[5] * 16777216;
					lPixelstart = zeiger->byHeader[10];
					lPixelstart = lPixelstart + zeiger->byHeader[11] * 256;
					lPixelstart = lPixelstart + zeiger->byHeader[12] * 65536;
					lPixelstart = lPixelstart + zeiger->byHeader[13] * 16777216;
					zeiger->iWidth = zeiger->byHeader[18];
					zeiger->iWidth = zeiger->iWidth + zeiger->byHeader[19] * 256;
					zeiger->iWidth = zeiger->iWidth + zeiger->byHeader[20] * 65536;
					zeiger->iWidth = zeiger->iWidth + zeiger->byHeader[21] * 16777216;
					zeiger->iHeight = zeiger->byHeader[22];
					zeiger->iHeight = zeiger->iHeight + zeiger->byHeader[23] * 256;
					zeiger->iHeight = zeiger->iHeight + zeiger->byHeader[24] * 65536;
					zeiger->iHeight = zeiger->iHeight + zeiger->byHeader[25] * 16777216;
					if (lDateilaenge != zeiger->iDateilaenge)
					{
						free(zeiger);
						fclose(datei);
						return ERROR_DAT;
					}
					zeiger->iHeaderlaengeExp = lPixelstart - zeiger->iHeaderlaenge;
					if (zeiger->iHeaderlaengeExp != 0)
					{
						if ((zeiger->byHeaderExp = (BYTE*)malloc(zeiger->iHeaderlaengeExp)) == NULL)
						{
							free(zeiger);
							fclose(datei);
							return ERROR_MEMO;
						}
						if (fread(zeiger->byHeaderExp, 1, zeiger->iHeaderlaengeExp, datei) != zeiger->iHeaderlaengeExp)
						{
							free(zeiger);
							fclose(datei);
							return ERROR_DAT;
						}
					}
					else
					{
						zeiger->byHeaderExp = NULL;
					}
					zeiger->iDatenlaenge = zeiger->iDateilaenge - lPixelstart;
					if ((zeiger->byData = (BYTE*)malloc(zeiger->iDatenlaenge)) == NULL)
					{
						free(zeiger);
						fclose(datei);
						return ERROR_MEMO;
					}
					else
					{
						if ((fread(zeiger->byData, 1, zeiger->iDatenlaenge, datei)) == zeiger->iDatenlaenge)
						{
							fclose(datei);
							zeiger->sFillByteCount = (zeiger->iDatenlaenge - (zeiger->iHeight*zeiger->iWidth * 3)) / zeiger->iHeight;
							return zeiger;
						}
						else
						{
							fclose(datei);
							return ERROR_DAT;
						}
					}
				}
			}
		}
		else
		{
			return ERROR_DAT;
		}
	}
	return zeiger;
}

int ins_crebm(stBitmap* bild)
{
	FILE *datei;
	long lData[3];

	if (bild == NULL)
		return ERROR_PT_ZERO;
	fopen_s(&datei, bild->cDateiname, "wb+");
	if (datei == NULL)
		return ERROR_DATA;
	lData[0] = fwrite(bild->byHeader, bild->iHeaderlaenge, 1, datei);
	lData[1] = fwrite(bild->byHeaderExp, bild->iHeaderlaengeExp, 1, datei);
	lData[2] = fwrite(bild->byData, bild->iDatenlaenge, 1, datei);
	return SUCCESS;
}

int ins_crebm(stBitmap* bild, char *newname)
{
	FILE *datei;
	long lData[3];

	if (bild == NULL)
		return ERROR_PT_ZERO;
	fopen_s(&datei, newname, "wb+");
	if (datei == NULL)
		return ERROR_DATA;
	lData[0] = fwrite((BYTE*)bild->byHeader, bild->iHeaderlaenge, 1, datei);
	if (bild->byHeaderExp != NULL)
		lData[1] = fwrite((BYTE*)bild->byHeaderExp, bild->iHeaderlaengeExp, 1, datei);
	lData[2] = fwrite((BYTE*)bild->byData, bild->iDatenlaenge, 1, datei);
	fclose(datei);
	return SUCCESS;
}

int ins_overwritehead(stBitmap* bild)
{
	bild->iHeaderlaenge = 54;
	bild->iHeaderlaengeExp = 0;
	bild->byHeader[0] = 'B'; //Magic
	bild->byHeader[1] = 'M'; //Numbers
	bild->byHeader[2] = (bild->iDateilaenge) & 0xFF;
	bild->byHeader[3] = (bild->iDateilaenge >> 8) & 0xFF;
	bild->byHeader[4] = (bild->iDateilaenge >> 16) & 0xFF;
	bild->byHeader[5] = (bild->iDateilaenge >> 24) & 0xFF;
	bild->byHeader[18] = (bild->iWidth) & 0xFF;
	bild->byHeader[19] = (bild->iWidth >> 8) & 0xFF;
	bild->byHeader[20] = (bild->iWidth >> 16) & 0xFF;
	bild->byHeader[21] = (bild->iWidth >> 24) & 0xFF;
	bild->byHeader[22] = (bild->iHeight) & 0xFF;
	bild->byHeader[23] = (bild->iHeight >> 8) & 0xFF;
	bild->byHeader[24] = (bild->iHeight >> 16) & 0xFF;
	bild->byHeader[25] = (bild->iHeight >> 24) & 0xFF;
	bild->byHeader[34] = (bild->iDatenlaenge) & 0xFF;
	bild->byHeader[35] = (bild->iDatenlaenge >> 8) & 0xFF;
	bild->byHeader[36] = (bild->iDatenlaenge >> 16) & 0xFF;
	bild->byHeader[37] = (bild->iDatenlaenge >> 24) & 0xFF;
	return SUCCESS;
}
