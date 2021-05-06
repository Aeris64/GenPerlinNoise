#ifndef PLAN_H
#define PLAN_H

#include "CustomNoise.h"

class Plan
{
public:
	float* map;

	const int mapWidth;
	const int mapHeight;
	const float scale;
	const int seed;

	const int octaves;
	const float persistance;
	const float lacunarity;

	int limitYMin;
	int limitYMax;
	float spawnRate;
	float value;
	float validValue;

	Plan(const int mapWidth, const int mapHeight, const float scale, const int seed, const int octaves, const float persistance, const float lacunarity);
	~Plan() = default;

	void SetSpawnPreference(const float newLimitYMin, const float newLimitYMax, const float newSpawnRate, const float newValue, const float newValidValue);
	void SetGenerateMap(CustomNoise newMap);
};

#endif // !PLAN_H