#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "CustomNoise.h"
#include "MapDisplay.h"

class MapGenerator
{
public:
	const int mapWidth;
	const int mapHeight;
	const float noiseScale;

	const int octaves;
	const float persistance;
	const float lacunarity;

	sf::Sprite* sprite;
	sf::Texture* texture;

	MapGenerator(const int mapWidth, const int mapHeight, const float noiseScale, const int octaves, const float persistance, const float lacunarity) :
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

	void GenerateMap()
	{
		// mapWidth = 1080
		// mapHeight = 720
		auto* noiseMapFinal = new float[mapWidth * mapHeight];

		CustomNoise Ground(mapWidth, 1, noiseScale, octaves, persistance, lacunarity);
		auto* noiseMapGround = Ground.noises;

		// Traverse the 2D array
		for (auto x = 0; x < mapWidth; x++)
		{
			// std::cout << "Y:" << y << std::endl;
			const float yCalcMax = mapHeight * *(noiseMapGround + 0 * mapWidth + x);

			for (auto y = 0; y < mapHeight; y++)
			{
				// std::cout << "X:" << x << std::endl;

				// Print values of the
				// memory block
				*(noiseMapFinal + y * mapWidth + x) = (y > yCalcMax ? 0 : 1);
			}
		}

		CustomNoise ns(mapWidth, mapHeight, noiseScale, octaves, persistance, lacunarity);
		auto* noiseMap = ns.noises;

		auto* pixels = new sf::Uint8[mapWidth * mapHeight * 4];

		MapDisplay::DrawNoiseMap(noiseMapFinal, mapWidth, mapHeight, pixels);

		/*// Traverse the 2D array
		for (auto y = 0; y < mapHeight; y++)
		{
			// std::cout << "Y:" << y << std::endl;

			for (auto x = 0; x < mapWidth; x++)
			{
				// std::cout << "X:" << x << std::endl;

				// Print values of the
				// memory block
				std::cout << *(noiseMap + y * mapWidth + x) << ", ";
			}
			std::cout << std::endl;
		}*/

		texture = new sf::Texture();

		if (!texture->create(mapWidth, mapHeight))
			std::cout << "err" << std::endl;

		texture->update(pixels);

		sprite = new sf::Sprite(*texture);
	}

	//void OnDraw(std::unique_ptr<sf::RenderWindow> m_pWindow)
	//{
	//	// On écrit notre image à l'écran.
	//	m_pWindow->draw(sprite);
	//}
};

#endif // !MAPGENERATOR_H
