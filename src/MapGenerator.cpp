#include "../include/MapGenerator.h"

#include "../include/CustomNoise.h"
#include "../include/MapDisplay.h"
#include "../include/PixelCheck.h"
#include "../include/Plan.h"

#include "nlohmann/json.hpp"
#include <./SFML/Graphics.hpp>
#include <fstream>
#include <iostream>


static const int answerLife = 42;
static const auto plansFile = "ressources/plans.json";

static const auto skyValue = 1.F;
static const auto floorValue = 0.F;

MapGenerator::MapGenerator(const int mapWidth, const int mapHeight, const float noiseScale, const int octaves, const float persistance, const float lacunarity, const int seed) :
	mapWidth((mapWidth < 1 ? 1 : mapWidth)),
	mapHeight((mapHeight < 1 ? 1 : mapHeight)),
	noiseScale((noiseScale < 0 ? 0.0009f : noiseScale)),
	octaves((octaves < 0 ? 0 : octaves)),
	persistance((persistance < 0 ? 0 : (persistance > 1 ? 1 : persistance))),
	lacunarity((lacunarity < 1 ? 1 : lacunarity)),
	seed(seed)
{
	sprite = nullptr;
	texture = nullptr;

	noiseMapFinal = new float[mapWidth * mapHeight];
}

void MapGenerator::GenerateMap()
{
	// Fill array of invalid value
	for (auto y = 0; y < mapHeight; y++) for (auto x = 0; x < mapWidth; x++) *(noiseMapFinal + y * mapWidth + x) = -1.f;

	// Create first 1 dimension noise for sky & ground
	CustomNoise noise(mapWidth, 1, noiseScale, octaves, persistance, lacunarity, seed);
	auto* noiseMapFloor = noise.GenerateNoise();
	const int limitYMountMin = mapHeight * 0.07F;
	const int limitYMountMax = mapHeight * 0.82F;

	// Read a JSON file
	std::ifstream i(plansFile);
	nlohmann::json json;
	i >> json;

	auto multiplier = seed;
	// Stock all noise map plan
	std::vector<Plan> allPlans;
	for (const auto& td : json)
	{
		const auto check = td["activate"];
		// Check if plan is activate
		if (!check) continue;

		// To differentiate in how many dimensions the plane is
		const int size = td["size"];
		const int mapWidthPlan = mapWidth;
		const int mapHeightPlan = (size == 1 ? 1 : mapHeight);
		const float scalePlan = td["scale"];
		const int octavesPlan = td["octaves"];
		const float persistancePlan = td["persistance"];
		const float lacunarityPlan = td["lacunarity"];

		// Set params for plan
		Plan actualPlan(mapWidthPlan, mapHeightPlan, scalePlan, multiplier, octavesPlan, persistancePlan, lacunarityPlan);

		const float limitYMinPlan = td["limitY"]["min"];
		const float limitYMaxPlan = td["limitY"]["max"];
		const float spawnRatePlan = td["spawn"]["rate"];
		const float valuePlan = td["spawn"]["value"];
		const float validValuePlan = td["spawn"]["validValue"];

		// Set params for plan
		actualPlan.SetSpawnPreference(limitYMinPlan, limitYMaxPlan, spawnRatePlan, valuePlan, validValuePlan);

		// Generate plan in map
		actualPlan.SetGenerateMap(noise);

		allPlans.push_back(actualPlan);

		multiplier *= answerLife;
	}

	// One dimension plan - WIP
	Plan snowPlan(mapWidth, 1, noiseScale, seed * seed, octaves, persistance, lacunarity);
	snowPlan.SetSpawnPreference(25, 55, 0.3F, 0.5F, floorValue);
	snowPlan.SetGenerateMap(noise);
	const int limitYSnowMin = mapHeight * 0.25F;
	const int limitYSnowMax = mapHeight * 0.55F;
	//

	// Traverse the 2D array
	for (auto x = 0; x < mapWidth; x++)
	{
		const auto yCalcMax = limitYMountMin + ((limitYMountMax - limitYMountMin) * (*(noiseMapFloor + 0 * mapWidth + x)));
		// One dimension plan - WIP
		const auto yCalcMaxSnow = limitYSnowMin + ((limitYSnowMax - limitYSnowMin) * (*(snowPlan.map + 0 * mapWidth + x)));
		//

		for (auto y = 0; y < mapHeight; y++)
		{
			auto actualValue = (y > yCalcMax ? floorValue : skyValue);

			// One dimension plan - WIP
			if (actualValue == floorValue && y < yCalcMaxSnow) actualValue = snowPlan.value;
			//

			// For all plans
			for (const auto& actualPlan : allPlans)
			{
				// Get noise value in plan
				const auto actualPlanValue = *(actualPlan.map + y * mapWidth + x);

				// If noise is okay, set value
				if (actualValue == actualPlan.validValue && actualPlanValue <= actualPlan.spawnRate) actualValue = actualPlan.value;
			}

			// Update value of memory block
			*(noiseMapFinal + y * mapWidth + x) = actualValue;
		}
	}

	// In order not to cut generation abruptly
	PixelCheck pixelSystem(noiseMapFinal, mapHeight, mapWidth);
	for (const auto& actualPlan : allPlans)
	{
		pixelSystem.SetParams(actualPlan);
		pixelSystem.GenerateCheck();
	}

	// Set pixels
	auto* pixels = new sf::Uint8[mapWidth * mapHeight * 4];
	// Draw pixels color
	MapDisplay::DrawNoiseMap(noiseMapFinal, mapWidth, mapHeight, pixels);
	texture = new sf::Texture();

	if (!texture->create(mapWidth, mapHeight))
		std::cout << "err" << std::endl;

	texture->update(pixels);

	sprite = new sf::Sprite(*texture);
}

sf::Sprite* MapGenerator::GetSprite()
{
	return sprite;
}