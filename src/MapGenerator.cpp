#include "../include/MapGenerator.h"

static const int answerLife = 42;

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

	Plan stonePlan(mapWidth, mapHeight, 100, seed / answerLife, 7, 0.15F, 20.F);
	stonePlan.SetSpawnPreference(70, 100, 0.4F, 0.25F);
	stonePlan.SetGenerateMap(noise);

	Plan cloudPlan(mapWidth, mapHeight, 200, seed * answerLife, 4, 0.55F, 3.F);
	cloudPlan.SetSpawnPreference(0, 10, 0.4F, 0.75F);
	cloudPlan.SetGenerateMap(noise);

	Plan snowPlan(mapWidth, 1, noiseScale, seed * seed, octaves, persistance, lacunarity);
	snowPlan.SetSpawnPreference(25, 55, 0.3F, 0.5F);
	snowPlan.SetGenerateMap(noise);
	const int limitYSnowMin = mapHeight * 0.25F;
	const int limitYSnowMax = mapHeight * 0.55F;

	Plan dirtPlan(mapWidth, mapHeight, 150, seed * answerLife / seed, 4, 0.15F, 20.F);
	dirtPlan.SetSpawnPreference(30, 100, 0.50F, 0.35F);
	dirtPlan.SetGenerateMap(noise);

	// Traverse the 2D array
	for (auto x = 0; x < mapWidth; x++)
	{
		// const auto yCalcMax = mapHeight * (*(noiseMapFloor + 0 * mapWidth + x));
		const auto yCalcMax = limitYMountMin + ((limitYMountMax - limitYMountMin) * (*(noiseMapFloor + 0 * mapWidth + x)));
		const auto yCalcMaxSnow = limitYSnowMin + ((limitYSnowMax - limitYSnowMin) * (*(snowPlan.map + 0 * mapWidth + x)));

		for (auto y = 0; y < mapHeight; y++)
		{
			auto actualValue = (y > yCalcMax ? floorValue : skyValue);

			const auto actualStoneValue = *(stonePlan.map + y * mapWidth + x);
			const auto actualDirtValue = *(dirtPlan.map + y * mapWidth + x);
			const auto actualCloudValue = *(cloudPlan.map + y * mapWidth + x);

			// If(..) then actualValue = (value < freqSpawn && spawn 25% Map (haut ou bas) alors ..)
			if (actualValue == floorValue && y < yCalcMaxSnow) actualValue = snowPlan.value;
			if (actualValue == floorValue && actualDirtValue <= dirtPlan.spawnRate) actualValue = dirtPlan.value;
			if (actualValue == floorValue && actualStoneValue <= stonePlan.spawnRate) actualValue = stonePlan.value;
			if (actualValue == skyValue && actualCloudValue <= cloudPlan.spawnRate) actualValue = cloudPlan.value;

			// Update value of memory block
			*(noiseMapFinal + y * mapWidth + x) = actualValue;
		}
	}

	PixelCheck pixelSystem(noiseMapFinal, mapHeight, mapWidth);
	pixelSystem.SetParams(stonePlan);
	pixelSystem.GenerateCheck();

	pixelSystem.SetParams(cloudPlan);
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