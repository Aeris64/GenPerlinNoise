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

	CustomNoise(const int mapWidth, const int mapHeight, float scale, const int octaves, const float persistance, const float lacunarity)
	{
		auto* noiseMap = new float[mapWidth * mapHeight];

		std::default_random_engine generator;
		std::uniform_int_distribution<int> distribution(1, 200);
		int dice_roll = distribution(generator);
		
		std::vector<sf::Vector2f> octaveOffsets(octaves);
		// auto* octaveOffsets = new sf::Vector2f();
		for (auto i = 0; i < octaves; i++)
		{
			const auto offSetX = distribution(generator);
			const auto offSetY = distribution(generator);

			octaveOffsets[i] = sf::Vector2f(offSetX, offSetY);
		}

		float maxNoiseHeight = -INFINITY;
		float minNoiseHeight = INFINITY;

		const float halfWidth = mapWidth / 2;
		const float halfHeight = mapHeight / 2;

		SimplexNoise simpleX;
		for (auto y = 0; y < mapHeight; y++)
		{
			for (auto x = 0; x < mapWidth; x++)
			{
				auto amplitude = 1.;
				auto frequency = 1.;
				auto noiseHeight = 0.;

				for (auto i = 0; i < octaves; i++)
				{
					const auto sampleX = (x - halfWidth) / scale * frequency + octaveOffsets[i].x;
					const auto sampleY = (y - halfHeight) / scale * frequency + octaveOffsets[i].y;
					/*const auto sampleX = (x - halfWidth) / scale * frequency;
					const auto sampleY = (y - halfHeight) / scale * frequency;*/

					const auto perlinValue = simpleX.noise(sampleX, sampleY) * 2 - 1;

					noiseHeight += perlinValue * amplitude;
					amplitude *= persistance;
					frequency *= lacunarity;
				}

				maxNoiseHeight = (noiseHeight > maxNoiseHeight ? noiseHeight : maxNoiseHeight);
				minNoiseHeight = (noiseHeight < minNoiseHeight ? noiseHeight : minNoiseHeight);

				*(noiseMap + y * mapWidth + x) = noiseHeight;

				// *(noiseMap + y * mapWidth + x) = perlinValue;
				// noiseMap[x, y] = perlinValue;
			}
		}

		for (auto y = 0; y < mapHeight; y++)
		{
			for (auto x = 0; x < mapWidth; x++)
			{
				*(noiseMap + y * mapWidth + x) = InvLerp(minNoiseHeight, maxNoiseHeight, *(noiseMap + y * mapWidth + x));

				// std::cout << *(noiseMap + y * mapWidth + x) << ", ";
			}
			// std::cout << std::endl;
		}

		noises = noiseMap;
	};

	~CustomNoise() = default;

	/*float Lerp(const float a, const float b, const float t)
	{
		return (1. - t) * a + b * t;
	}*/

	float InvLerp(const float a, const float b, const float t)
	{
		return (t - a) / (b - a);
	}
};

#endif // !CUSTOMNOISE_H