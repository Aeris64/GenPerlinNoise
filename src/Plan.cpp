#include "../include/Plan.h"

Plan::Plan(const int mapWidth, const int mapHeight, const float scale, const int seed, const int octaves, const float persistance, const float lacunarity) :
	mapWidth((mapWidth < 1 ? 1 : mapWidth)),
	mapHeight((mapHeight < 1 ? 1 : mapHeight)),
	scale((scale < 0 ? 0.0009f : scale)),
	seed(seed),
	octaves((octaves < 0 ? 0 : octaves)),
	persistance((persistance < 0 ? 0 : (persistance > 1 ? 1 : persistance))),
	lacunarity((lacunarity < 1 ? 1 : lacunarity)),
	limitYMin(0),
	limitYMax(0),
	spawnRate(0.F),
	value(0.F)
{
	map = nullptr;
}

void Plan::SetSpawnPreference(const float newLimitYMin, const float newLimitYMax, const float newSpawnRate, const float newValue)
{
	limitYMin = ((newLimitYMin < 0.F ? 0 : (newLimitYMin > 100.F ? 100 : newLimitYMin)) / 100.F) * mapHeight;
	limitYMax = ((newLimitYMax < 0.F ? 0 : (newLimitYMax > 100.F ? 100 : newLimitYMax)) / 100.F) * mapHeight;
	spawnRate = ((newSpawnRate < 0 ? 0 : (newSpawnRate > 1 ? 1 : newSpawnRate)));
	value = ((newValue < 0 ? 0 : (newValue > 1 ? 1 : newValue)));
}

void Plan::SetGenerateMap(CustomNoise noise)
{
	noise.SetWidth(mapWidth);
	noise.SetHeight(mapHeight);
	noise.SetScale(scale);
	noise.SetOctaves(octaves);
	noise.SetSeed(seed);
	noise.SetPersistance(persistance);
	noise.SetLacunarity(lacunarity);

	map = noise.GenerateNoise();
}