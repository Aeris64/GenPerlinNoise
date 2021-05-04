#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "CustomNoise.h"
#include "MapDisplay.h"
#include "PixelCheck.h"
#include "Plan.h"

class MapGenerator
{
public:
	float* noiseMapFinal;

	const int mapWidth;
	const int mapHeight;
	const float noiseScale;
	const int seed;

	const int octaves;
	const float persistance;
	const float lacunarity;

	sf::Sprite* sprite;
	sf::Texture* texture;

	MapGenerator(const int mapWidth, const int mapHeight, const float noiseScale, const int octaves, const float persistance, const float lacunarity, const int seed);

	void GenerateMap();

private:
	void devLimitCheck(float* map, const int limitYMin, const int limitYMax, const float value, const int nb, const int mapWidth);

	//void OnDraw(std::unique_ptr<sf::RenderWindow> m_pWindow)
	//{
	//	// On �crit notre image � l'�cran.
	//	m_pWindow->draw(sprite);
	//}
};

#endif // !MAPGENERATOR_H
