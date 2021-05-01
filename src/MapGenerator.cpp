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

	// Traverse the 2D array
	for (auto x = 0; x < mapWidth; x++)
	{
		const float yCalcMax = mapHeight * (*(noiseMapFloor + 0 * mapWidth + x));

		for (auto y = 0; y < mapHeight; y++)
		{
			auto actualValue = (y > yCalcMax ? 0.F : 1.F);

			// If(..) then actualValue = (value < freqSpawn && spawn 25% Map (haut ou bas) alors ..)
			if (actualValue < 0.5) actualValue = ((*(noiseMapCave + y * mapWidth + x) <= 0.4F) && (y > mapWidth * 0.50) ? 0.25F : 0.F);
			if (actualValue > 0.5) actualValue = ((*(noiseMapCloud + y * mapWidth + x) <= 0.30F) && (y < mapWidth * 0.10) ? 0.75F : 1.F);

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