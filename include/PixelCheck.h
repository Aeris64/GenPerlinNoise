#ifndef PIXELCHECK_H
#define PIXELCHECK_H

#include "Plan.h"

class PixelCheck
{
public:
	float* map;

	const int mapHeight;
	const int mapWidth;

	float valueCheck;
	int limitYMax;
	int limitYMin;

	float defaultValue;

	PixelCheck(float* map, const int mapHeight, const int mapWidth);
	float* GenerateCheck();
	void SetParams(Plan actualPlan);

private:
	bool* mapBool;

	bool PixelAllCheck(const int x, const int y);

	bool PixelYCheck(int x, int y);
	bool PixelTopSubCheck(int x, int y);
	bool PixelBottomSubCheck(int x, int y);

	bool PixelLeftCheck(int x, int y);
	bool PixelRightCheck(int x, int y);
	bool PixelBottomCheck(int x, int y);
};

#endif // !PIXELCHECK_H