#include "../include/CustomNoise.h"
#include "../include/SimplexNoise.h"
#include "../include/MapDisplay.h"

#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <random>
#include <vector>

CustomNoise::CustomNoise(const int mapWidth, const int mapHeight, float scale, const int octaves, const float persistance, const float lacunarity, const int seed) :
	mapWidth((mapWidth < 1 ? 1 : mapWidth)),
	mapHeight((mapHeight < 1 ? 1 : mapHeight)),
	scale((scale < 0 ? 0.0009f : scale)),
	octaves((octaves < 0 ? 0 : octaves)),
	persistance((persistance < 0 ? 0 : (persistance > 1 ? 1 : persistance))),
	lacunarity((lacunarity < 1 ? 1 : lacunarity)),
	seed(seed),
	noises(nullptr)
{}

float* CustomNoise::GenerateNoise()
{
	auto* noiseMap = new float[mapWidth * mapHeight];

	//std::default_random_engine generator;
	//std::uniform_int_distribution<int> distribution(1, 500);
	//int dice_roll = distribution(generator);

	//std::vector<sf::Vector2f> octaveOffsets(octaves);
	//// auto* octaveOffsets = new sf::Vector2f();
	//for (auto i = 0; i < octaves; i++)
	//{
	//	const auto offSetX = distribution(generator);
	//	const auto offSetY = distribution(generator);

	//	octaveOffsets[i] = sf::Vector2f(offSetX, offSetY);
	//}

	float maxNoiseHeight = -INFINITY;
	float minNoiseHeight = INFINITY;

	SimplexNoise simpleX;
	simpleX.mLacunarity = lacunarity;
	simpleX.mPersistence = persistance;
	simpleX.mSeed = seed;

	for (auto y = 0; y < mapHeight; y++)
	{
		const auto sampleY = y / scale;

		for (auto x = 0; x < mapWidth; x++)
		{
			const auto sampleX = x / scale;

			/*auto amplitude = 1.;
			auto frequency = 1.;
			auto noiseHeight = 0.;

			for (auto i = 0; i < octaves; i++)
			{
				const auto sampleX = (x - halfWidth) / scale * frequency + octaveOffsets[i].x;
				const auto sampleY = (y - halfHeight) / scale * frequency + octaveOffsets[i].y;
				/*const auto sampleX = (x - halfWidth) / scale * frequency;
				const auto sampleY = (y - halfHeight) / scale * frequency;* /

				const auto perlinValue = simpleX.noise(sampleX, sampleY) * 2 - 1;

				noiseHeight += perlinValue * amplitude;
				amplitude *= persistance;
				frequency *= lacunarity;
			}

			maxNoiseHeight = (noiseHeight > maxNoiseHeight ? noiseHeight : maxNoiseHeight);
			minNoiseHeight = (noiseHeight < minNoiseHeight ? noiseHeight : minNoiseHeight);

			*(noiseMap + y * mapWidth + x) = noiseHeight;*/

			const auto perlinValue = simpleX.fractal(octaves, sampleX, sampleY) * 2 - 1;
			*(noiseMap + y * mapWidth + x) = perlinValue;

			maxNoiseHeight = (perlinValue > maxNoiseHeight ? perlinValue : maxNoiseHeight);
			minNoiseHeight = (perlinValue < minNoiseHeight ? perlinValue : minNoiseHeight);
		}
	}

	// Lerp of the values in relation to the minimum and maximum values
	for (auto y = 0; y < mapHeight; y++)
	{
		for (auto x = 0; x < mapWidth; x++)
		{
			*(noiseMap + y * mapWidth + x) = InvLerp(minNoiseHeight, maxNoiseHeight, *(noiseMap + y * mapWidth + x));
		}
	}

	noises = noiseMap;
	return noiseMap;
};

float CustomNoise::Lerp(const float a, const float b, const float t)
{
	return (1. - t) * a + b * t;
}

float CustomNoise::InvLerp(const float a, const float b, const float t)
{
	return (t - a) / (b - a);
}

void CustomNoise::SetWidth(const int newWidth)
{
	mapWidth = (newWidth < 1 ? 1 : newWidth);
}

void CustomNoise::SetHeight(const int newHeight)
{
	mapHeight = (newHeight < 1 ? 1 : newHeight);
}

void CustomNoise::SetScale(const float newScale)
{
	scale = (newScale < 0 ? 0.0009f : newScale);
}

void CustomNoise::SetOctaves(const int newOctaves)
{
	octaves = (newOctaves < 0 ? 0 : newOctaves);
}

void CustomNoise::SetSeed(const int newSeed)
{
	seed = newSeed;
}

void CustomNoise::SetPersistance(const float newPersistance)
{
	persistance = (newPersistance < 0 ? 0 : (newPersistance > 1 ? 1 : newPersistance));
}

void CustomNoise::SetLacunarity(const float newLacunarity)
{
	lacunarity = (newLacunarity < 1 ? 1 : newLacunarity);
}