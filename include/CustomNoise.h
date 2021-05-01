#ifndef CUSTOMNOISE_H
#define CUSTOMNOISE_H

#include <iostream>

#include "SimplexNoise.h"
#include "MapDisplay.h"
#include <SFML/System/Vector2.hpp>
#include <random>
#include <vector>

class CustomNoise
{
public:
	float* noises;

	int mapWidth;
	int mapHeight;
	float scale;
	int octaves;
	float persistance;
	float lacunarity;

	CustomNoise(const int mapWidth, const int mapHeight, float scale, const int octaves, const float persistance, const float lacunarity);
	~CustomNoise() = default;

	float* GenerateNoise();

	float Lerp(const float a, const float b, const float t);
	float InvLerp(const float a, const float b, const float t);

	void SetWidth(int newWidth);
	void SetHeight(int newHeight);
	void SetScale(float newScale);
	void SetOctaves(int newOctaves);
	void SetPersistance(float newPersistance);
	void SetLacunarity(float newLacunarity);
};

#endif // !CUSTOMNOISE_H