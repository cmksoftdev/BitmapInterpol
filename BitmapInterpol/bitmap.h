#pragma once
#include <stdio.h>
#include <stdlib.h>

/*Fehlerdefinitionen Int*/		//English				//Deutsch
#define ERROR_DATA -1;			//File error			//Datei Fehler.
#define ERROR_DAT_EMPTY -1;		//File empty			//Datei konnte nicht gelesen werden.
#define ERROR_PT_ZERO -1;		//Zero Pointer			//Zeiger ist NULL.
#define ERROR_NEM -1;			//Not enough memory		//Speicher zu klein.
#define ERROR_POS -1;			//Position error		//Positionsfehler.

/*Fehlerdefinitionen Zeiger*/
#define ERROR_DAT NULL;				//Could not open file			//Datei konnte nicht gelesen werden.
#define ERROR_MEMO NULL;			//Could not allocate memory		//Fehler bei Speicherreservierung.
#define ERROR_READ NULL;			//Could not read file till end	//Die Datei wurde nicht vollständig gelesen.
#define ERROR_PT NULL;				//Zero pointer error			//Zeiger ist NULL.

#define SUCCESS 0;					//Exit success	//Funktion erfolgreich durchlaufen.

typedef unsigned char BYTE;

struct stBitmap
{
	char *cDateiname;
	unsigned int iDateilaenge;
	unsigned int iHeaderlaenge;
	unsigned int iHeaderlaengeExp;
	unsigned int iDatenlaenge;
	unsigned int iWidth;
	unsigned int iHeight;
	unsigned short sFillByteCount;
	BYTE *byHeader;
	BYTE *byHeaderExp;
	BYTE *byData;
};

stBitmap* ins_chopbm(char* dat);
int ins_crebm(stBitmap* bild);
int ins_crebm(stBitmap* bild, char *newname);
int ins_overwritehead(stBitmap* bild);
