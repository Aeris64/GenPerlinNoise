#include "../include/MapGenerator.h"

MapGenerator::MapGenerator(const int mapWidth, const int mapHeight, const float noiseScale, const int octaves, const float persistance, const float lacunarity) :
	mapWidth((mapWidth < 1 ? 1 : mapWidth)),
	mapHeight((mapHeight < 1 ? 1 : mapHeight)),
	noiseScale((noiseScale < 0 ? 0.0009f : noiseScale)),
	octaves((octaves < 0 ? 0 : octaves)),
	persistance((persistance < 0 ? 0 : (persistance > 1 ? 1 : persistance))),
	lacunarity((lacunarity < 1 ? 1 : lacunarity))
{
	sprite = nullptr;
	texture = nullptr;
}

void MapGenerator::GenerateMap()
{
	auto* noiseMapFinal = new float[mapWidth * mapHeight];
	for (auto y = 0; y < mapHeight; y++) for (auto x = 0; x < mapWidth; x++) *(noiseMapFinal + y * mapWidth + x) = -1.f;

	CustomNoise noise(mapWidth, 1, noiseScale, octaves, persistance, lacunarity);	
	auto* noiseMapFloor = noise.GenerateNoise();

	noise.SetHeight(mapHeight);
	noise.SetScale(150);
	noise.SetOctaves(4);
	noise.SetPersistance(1.f);
	noise.SetLacunarity(3000.f);
	auto* noiseMapCave = noise.GenerateNoise();

	noise.SetHeight(mapHeight);
	noise.SetScale(500);
	noise.SetOctaves(5);
	auto* noiseMapCloud = noise.GenerateNoise();
	const auto limitYCloud = mapHeight * 0.1F;
	const auto spawnRateCloud = 0.3F;
	const auto cloudValue = 0.75F;

	// Traverse the 2D array
	for (auto x = 0; x < mapWidth; x++)
	{
		const float yCalcMax = mapHeight * (*(noiseMapFloor + 0 * mapWidth + x));

		for (auto y = 0; y < mapHeight; y++)
		{
			auto actualValue = (y > yCalcMax ? 0.F : 1.F);

			const auto topValue = *(noiseMapFinal + (y == 0 ? 0 : y - 1) * mapWidth + x);
			const auto leftValue = *(noiseMapFinal + y * mapWidth + (x == 0 ? 0 : x - 1));

			const auto actualCloudValue = *(noiseMapCloud + y * mapWidth + x);

			// If(..) then actualValue = (value < freqSpawn && spawn 25% Map (haut ou bas) alors ..)
			if (actualValue < 0.5) actualValue = ((*(noiseMapCave + y * mapWidth + x) <= 0.4F) && (y > mapWidth * 0.50) ? 0.25F : 0.F);
			if (actualValue > 0.5 && actualCloudValue <= spawnRateCloud) {
				const auto bottomValueCloud = *(noiseMapCloud + (y == mapHeight ? mapHeight : y + 1) * mapWidth + x);
				const auto rightValueCloud = *(noiseMapCloud + y * mapWidth + (x == mapWidth ? mapWidth : x + 1));

				if (y < limitYCloud) actualValue = cloudValue;
				else if(topValue == cloudValue
					|| leftValue == cloudValue
					|| (bottomValueCloud > 0.5 && bottomValueCloud <= spawnRateCloud) 
					|| (rightValueCloud > 0.5 && rightValueCloud <= spawnRateCloud))
				actualValue = cloudValue;
			}

			// Update value of memory block
			*(noiseMapFinal + y * mapWidth + x) = actualValue;
		}
	}

	auto* pixels = new sf::Uint8[mapWidth * mapHeight * 4];

	MapDisplay::DrawNoiseMap(noiseMapFinal, mapWidth, mapHeight, pixels);

	/*// Traverse the 2D array
	for (auto y = 0; y < 3; y++)
	{
		// std::cout << "Y:" << y << std::endl;

		for (auto x = 0; x < 3; x++)
		{
			// std::cout << "X:" << x << std::endl;

			// Print values of the
			// memory block
			std::cout << *(noiseMapFinal + y * mapWidth + x) << ", ";
		}
		std::cout << std::endl;
	}*/

	texture = new sf::Texture();

	if (!texture->create(mapWidth, mapHeight))
		std::cout << "err" << std::endl;

	texture->update(pixels);

	sprite = new sf::Sprite(*texture);
}