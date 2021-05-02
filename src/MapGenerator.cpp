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

	noiseMapFinal = new float[mapWidth * mapHeight];
}

void MapGenerator::GenerateMap()
{
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
	const int limitYCloud = mapHeight * 0.1F;
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
			if (actualValue > 0.5 && actualCloudValue <= spawnRateCloud) actualValue = cloudValue;

			// Update value of memory block
			*(noiseMapFinal + y * mapWidth + x) = actualValue;
		}
	}

	PixelCheck(cloudValue, limitYCloud);

	// TEMP
	for (auto y = 0; y < 5; y++)
	{
		for (auto x = 0; x < mapWidth; x++)
		{
			*(noiseMapFinal + (limitYCloud + y) * mapWidth + x) = 0.25F;
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

void MapGenerator::PixelCheck(const float valueCheck, const int limitY)
{
	for (auto y = limitY + 1; y < mapHeight; y++)
	{
		for (auto x = 0; x < mapWidth; x++)
		{
			const auto actualValue = *(noiseMapFinal + y * mapWidth + x);

			if (actualValue == valueCheck)
			{
				const auto r = PixelAllCheck(x, y, valueCheck, limitY);
				if (!r) *(noiseMapFinal + y * mapWidth + x) = 1.F;
			}
		}
	}
}

bool MapGenerator::PixelAllCheck(const int x, const int y, const float valueCheck, const int limitY)
{
	auto res = false;

	if (PixelTopCheck(x, y, valueCheck, limitY)) res = true;
	else if (PixelLeftCheck(x, y, valueCheck, limitY)) res = true;
	else if (PixelRightCheck(x, y, valueCheck, limitY)) res = true;
	else if (PixelBottomCheck(x, y, valueCheck, limitY)) res = true;

	return res;
}

bool MapGenerator::PixelTopCheck(int x, int y, const float valueCheck, const int limitY)
{
	auto res = true;
	auto actualValue = *(noiseMapFinal + y * mapWidth + x);
	while (actualValue == valueCheck && y > limitY)
	{
		y--;
		actualValue = *(noiseMapFinal + y * mapWidth + x);
	}

	if (actualValue != valueCheck) res = false;

	return res;
}

bool MapGenerator::PixelLeftCheck(int x, int y, const float valueCheck, const int limitY)
{
	auto res = false;
	auto actualValue = *(noiseMapFinal + y * mapWidth + x);
	do {
		if (actualValue == valueCheck && PixelTopCheck(x, y, valueCheck, limitY)) res = true;
		actualValue = *(noiseMapFinal + y * mapWidth + x);

		x--;
	} while (actualValue == valueCheck && x >= (0 - 1) && (!res));

	return res;
}

bool MapGenerator::PixelRightCheck(int x, int y, const float valueCheck, const int limitY)
{
	auto res = false;
	auto actualValue = *(noiseMapFinal + y * mapWidth + x);
	do {
		if (actualValue == valueCheck && PixelTopCheck(x, y, valueCheck, limitY)) res = true;
		actualValue = *(noiseMapFinal + y * mapWidth + x);

		x++;
	} while (actualValue == valueCheck && x <= (mapWidth + 1) && (!res));

	return res;
}

bool MapGenerator::PixelBottomCheck(int x, int y, const float valueCheck, const int limitY)
{
	auto res = false;
	auto actualValue = *(noiseMapFinal + y * mapWidth + x);
	do {
		if (actualValue == valueCheck)
		{
			if(PixelLeftCheck(x, y, valueCheck, limitY)) res = true;
			else if (PixelRightCheck(x, y, valueCheck, limitY)) res = true;
		}
		actualValue = *(noiseMapFinal + y * mapWidth + x);

		y++;
	} while (actualValue == valueCheck && y <= (mapHeight + 1) && (!res));

	return res;
}

/*bool MapGenerator::PixelBottomCheck(int x, int y, const float valueCheck, const int limitY)
{
	auto res = false;
	auto actualValue = *(noiseMapFinal + y * mapWidth + x);
	if (actualValue == valueCheck && (PixelLeftCheck(x, y, valueCheck, limitY) || PixelRightCheck(x, y, valueCheck, limitY))) res = true;

	while (actualValue == valueCheck && y < mapHeight && (!res))
	{
		y++;
		actualValue = *(noiseMapFinal + y * mapWidth + x);

		if (actualValue == valueCheck && (PixelLeftCheck(x, y, valueCheck, limitY) || PixelRightCheck(x, y, valueCheck, limitY))) res = true;
	}

	return res;
}*/