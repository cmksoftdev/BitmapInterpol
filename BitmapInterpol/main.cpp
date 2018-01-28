#include "engine.h"

void main()
{
	stBitmap* bmIn;
	bmIn = ins_chopbm("in.bmp");
	InterpolationEngine engine(*bmIn);
	engine.DoubleSolution("out.bmp");
}
