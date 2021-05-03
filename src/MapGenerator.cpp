#include "../include/MapGenerator.h"

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
	for (auto y = 0; y < mapHeight; y++) for (auto x = 0; x < mapWidth; x++) *(noiseMapFinal + y * mapWidth + x) = -1.f;

	CustomNoise noise(mapWidth, 1, noiseScale, octaves, persistance, lacunarity, seed);
	auto* noiseMapFloor = noise.GenerateNoise();
	const int limitYMountMin = mapHeight * 0.07F;
	const int limitYMountMax = mapHeight * 0.82F;

	const auto skyValue = 1.F;
	const auto floorValue = 0.F;

	noise.SetHeight(mapHeight);
	/*noise.SetScale(150);
	noise.SetOctaves(4);*/
	noise.SetScale(75);
	noise.SetOctaves(4);
	noise.SetPersistance(0.5f);
	noise.SetLacunarity(20.f);
	auto* noiseMapStone = noise.GenerateNoise();
	const int limitYStoneMin = mapHeight * 0.7F;
	const int limitYStoneMax = mapHeight * 1.F - 1;
	const auto spawnRateStone = 0.4F;
	const auto stoneValue = 0.25F;

	noise.SetHeight(mapHeight);
	/*noise.SetScale(500);
	noise.SetOctaves(5);*/
	noise.SetScale(200);
	noise.SetOctaves(7);
	noise.SetPersistance(0.55f);
	noise.SetLacunarity(2.f);
	noise.SetSeed(seed * 300);
	auto* noiseMapCloud = noise.GenerateNoise();
	const int limitYCloudMin = mapHeight * 0.F;
	const int limitYCloudMax = mapHeight * 0.1F;
	const auto spawnRateCloud = 0.35F;
	const auto cloudValue = 0.75F;

	noise.SetHeight(1);
	noise.SetScale(noiseScale);
	noise.SetOctaves(octaves);
	noise.SetPersistance(persistance);
	noise.SetLacunarity(lacunarity);
	noise.SetSeed(seed * seed);
	auto* noiseMapSnow = noise.GenerateNoise();
	const int limitYSnowMin = mapHeight * 0.25F;
	const int limitYSnowMax = mapHeight * 0.55F;
	const auto spawnRateSnow = 0.3F;
	const auto snowValue = 0.5F;

	// Traverse the 2D array
	for (auto x = 0; x < mapWidth; x++)
	{
		// const auto yCalcMax = mapHeight * (*(noiseMapFloor + 0 * mapWidth + x));
		const auto yCalcMax = limitYMountMin + ((limitYMountMax - limitYMountMin) * (*(noiseMapFloor + 0 * mapWidth + x)));
		const auto yCalcMaxSnow = limitYSnowMin + ((limitYSnowMax - limitYSnowMin) * (*(noiseMapSnow + 0 * mapWidth + x)));

		for (auto y = 0; y < mapHeight; y++)
		{
			auto actualValue = (y > yCalcMax ? floorValue : skyValue);

			const auto actualCloudValue = *(noiseMapCloud + y * mapWidth + x);
			const auto actualStoneValue = *(noiseMapStone + y * mapWidth + x);

			// If(..) then actualValue = (value < freqSpawn && spawn 25% Map (haut ou bas) alors ..)
			if (actualValue == floorValue && y < yCalcMaxSnow) actualValue = snowValue;
			if (actualValue == floorValue && actualStoneValue <= spawnRateStone) actualValue = stoneValue;
			if (actualValue == skyValue && actualCloudValue <= spawnRateCloud) actualValue = cloudValue;

			// Update value of memory block
			*(noiseMapFinal + y * mapWidth + x) = actualValue;
		}
	}

	PixelCheck pixelSystem(noiseMapFinal, cloudValue, limitYCloudMin, limitYCloudMax, mapHeight, mapWidth, skyValue);
	pixelSystem.GenerateCheck();

	pixelSystem.SetParams(stoneValue, limitYStoneMin, limitYStoneMax, floorValue);
	pixelSystem.GenerateCheck();

	// TEMP
	//devLimitCheck(noiseMapFinal, limitYMountMin, limitYMountMax, stoneValue, 5, mapWidth);
	//devLimitCheck(noiseMapFinal, limitYCaveMin, limitYCaveMax, cloudValue, 5, mapWidth);

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