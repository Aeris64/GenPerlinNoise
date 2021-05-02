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

	const auto skyValue = 1.F;
	const auto floorValue = 0.F;

	noise.SetHeight(mapHeight);
	/*noise.SetScale(150);
	noise.SetOctaves(4);*/
	noise.SetScale(75);
	noise.SetOctaves(4);
	noise.SetPersistance(1.f);
	noise.SetLacunarity(3000.f);
	auto* noiseMapCave = noise.GenerateNoise();
	const int limitYCaveMin = mapHeight * 0.7F;
	const int limitYCaveMax = mapHeight * 1.F - 1;
	const auto spawnRateCave = 0.4F;
	const auto caveValue = 0.25F;

	noise.SetHeight(mapHeight);
	/*noise.SetScale(500);
	noise.SetOctaves(5);*/
	noise.SetScale(200);
	noise.SetOctaves(1);
	auto* noiseMapCloud = noise.GenerateNoise();
	const int limitYCloudMin = mapHeight * 0.;
	const int limitYCloudMax = mapHeight * 0.1F;
	const auto spawnRateCloud = 0.3F;
	const auto cloudValue = 0.75F;

	// Traverse the 2D array
	for (auto x = 0; x < mapWidth; x++)
	{
		const float yCalcMax = mapHeight * (*(noiseMapFloor + 0 * mapWidth + x));

		for (auto y = 0; y < mapHeight; y++)
		{
			auto actualValue = (y > yCalcMax ? floorValue : skyValue);

			const auto topValue = *(noiseMapFinal + (y == 0 ? 0 : y - 1) * mapWidth + x);
			const auto leftValue = *(noiseMapFinal + y * mapWidth + (x == 0 ? 0 : x - 1));

			const auto actualCloudValue = *(noiseMapCloud + y * mapWidth + x);
			const auto actualCaveValue = *(noiseMapCave + y * mapWidth + x);

			// If(..) then actualValue = (value < freqSpawn && spawn 25% Map (haut ou bas) alors ..)
			if (actualValue < 0.5 && actualCaveValue <= spawnRateCave) actualValue = caveValue;
			if (actualValue > 0.5 && actualCloudValue <= spawnRateCloud) actualValue = cloudValue;

			// Update value of memory block
			*(noiseMapFinal + y * mapWidth + x) = actualValue;
		}
	}

	PixelCheck pixelSystem(noiseMapFinal, cloudValue, limitYCloudMin, limitYCloudMax, mapHeight, mapWidth, skyValue);
	pixelSystem.GenerateCheck();

	pixelSystem.SetParams(caveValue, limitYCaveMin, limitYCaveMax, floorValue);
	pixelSystem.GenerateCheck();

	// TEMP
	//devLimitCheck(noiseMapFinal, limitYCloudMin, limitYCloudMax, caveValue, 5, mapWidth);
	devLimitCheck(noiseMapFinal, limitYCaveMin, limitYCaveMax, cloudValue, 5, mapWidth);

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

void MapGenerator::devLimitCheck(float* map, const int limitYMin, const int limitYMax, const float value, const int nb, const int mapWidth)
{
	for (auto y = 0; y < nb; y++)
	{
		for (auto x = 0; x < mapWidth; x++)
		{
			*(map + (limitYMax - y) * mapWidth + x) = value;
		}
	}
	for (auto y = 0; y < nb; y++)
	{
		for (auto x = 0; x < mapWidth; x++)
		{
			*(map + (limitYMin + y) * mapWidth + x) = value;
		}
	}
}