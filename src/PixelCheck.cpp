#include "../include/PixelCheck.h"

PixelCheck::PixelCheck(float* map, const int mapHeight, const int mapWidth) :
	map(map),
	valueCheck(0.F),
	limitYMin(0),
	limitYMax(0),
	mapHeight(mapHeight),
	mapWidth(mapWidth),
	defaultValue(0.F)
{
	mapBool = new bool[mapWidth * mapHeight];
}

float* PixelCheck::GenerateCheck()
{
	// Filling mapBool at false
	for (auto y = 0; y < mapHeight; y++) for (auto x = 0; x < mapWidth; x++) *(mapBool + y * mapWidth + x) = false;

	for (auto y = 0 + 1; y < mapHeight; y++)
	{
		if (y >= limitYMin && y <= limitYMax) continue;

		for (auto x = 0; x < mapWidth; x++)
		{
			const auto actualValue = *(map + y * mapWidth + x);

			if (actualValue == valueCheck)
			{
				const auto r = PixelAllCheck(x, y);
				if (!r) *(map + y * mapWidth + x) = defaultValue;
				else *(mapBool + y * mapWidth + x) = true;
			}
		}
	}

	return map;
}

void PixelCheck::SetParams(Plan actualPlan)
{
	valueCheck = actualPlan.value;
	limitYMin = actualPlan.limitYMin;
	limitYMax = actualPlan.limitYMax;
	defaultValue = (valueCheck < 0.5 ? 0 : 1);
}

bool PixelCheck::PixelAllCheck(const int x, const int y)
{
	auto res = false;

	if (*(mapBool + y * mapWidth + x)) res = true;

	if (PixelYCheck(x, y)) res = true;
	else if (PixelLeftCheck(x, y)) res = true;
	else if (PixelRightCheck(x, y)) res = true;
	// else if (PixelBottomCheck(x, y)) res = true;

	if(res) *(mapBool + y * mapWidth + x) = true;

	return res;
}

/*bool PixelCheck::PixelTopCheck(int x, int y, const float valueCheck, const int limitY)
{
	auto res = true;
	auto actualValue = *(map + y * mapWidth + x);
	while (actualValue == valueCheck && y > limitY)
	{
		if (*(mapBool + y * mapWidth + x)) res = true;
		y--;
		actualValue = *(map + y * mapWidth + x);
	}

	if (actualValue != valueCheck) res = false;

	if(res) *(mapBool + y * mapWidth + x) = true;

	return res;
}*/
bool PixelCheck::PixelYCheck(int x, int y)
{
	if (y <= limitYMin) return PixelBottomSubCheck(x, y);
	return PixelTopSubCheck(x, y);
}

bool PixelCheck::PixelTopSubCheck(int x, int y)
{
	auto res = false;
	auto actualValue = *(map + y * mapWidth + x);
	do {
		if (*(mapBool + y * mapWidth + x)) res = true;

		actualValue = *(map + y * mapWidth + x);

		y--;
	} while (actualValue == valueCheck && y >= limitYMax && (!res));

	if (actualValue == valueCheck) {
		res = true;
		*(mapBool + y * mapWidth + x) = true;
	}

	return res;
}

bool PixelCheck::PixelBottomSubCheck(int x, int y)
{
	auto res = false;
	auto actualValue = *(map + y * mapWidth + x);
	do {
		if (*(mapBool + y * mapWidth + x)) res = true;

		actualValue = *(map + y * mapWidth + x);

		y++;
	} while (actualValue == valueCheck && y <= limitYMin && (!res));

	if (actualValue == valueCheck) {
		res = true;
		*(mapBool + y * mapWidth + x) = true;
	}

	return res;
}

bool PixelCheck::PixelLeftCheck(int x, int y)
{
	auto res = false;
	auto actualValue = *(map + y * mapWidth + x);
	do {
		if (*(mapBool + y * mapWidth + x)) res = true;

		if ((!res) && actualValue == valueCheck && PixelYCheck(x, y)) {
			res = true;
			*(mapBool + y * mapWidth + x) = true;
		}
		actualValue = *(map + y * mapWidth + x);

		x--;
	} while (actualValue == valueCheck && x >= (0 - 1) && (!res));

	return res;
}

bool PixelCheck::PixelRightCheck(int x, int y)
{
	auto res = false;
	auto actualValue = *(map + y * mapWidth + x);
	do {
		if (*(mapBool + y * mapWidth + x)) res = true;

		if ((!res) && actualValue == valueCheck && PixelYCheck(x, y)) {
			res = true;
			*(mapBool + y * mapWidth + x) = true;
		}
		actualValue = *(map + y * mapWidth + x);

		x++;
	} while (actualValue == valueCheck && x <= (mapWidth + 1) && (!res));

	return res;
}

bool PixelCheck::PixelBottomCheck(int x, int y)
{
	auto res = false;
	auto actualValue = *(map + y * mapWidth + x);
	do {
		if (*(mapBool + y * mapWidth + x)) res = true;

		if ((!res) && actualValue == valueCheck)
		{
			if (PixelLeftCheck(x, y)) res = true;
			else if (PixelRightCheck(x, y)) res = true;

			if(res) *(mapBool + y * mapWidth + x) = true;
		}
		actualValue = *(map + y * mapWidth + x);

		y++;
	} while (actualValue == valueCheck && y <= (mapHeight + 1) && (!res));

	return res;
}