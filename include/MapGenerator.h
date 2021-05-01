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

	MapGenerator(const int mapWidth, const int mapHeight, const float noiseScale, const int octaves, const float persistance, const float lacunarity);

	void GenerateMap();

	//void OnDraw(std::unique_ptr<sf::RenderWindow> m_pWindow)
	//{
	//	// On �crit notre image � l'�cran.
	//	m_pWindow->draw(sprite);
	//}
};

#endif // !MAPGENERATOR_H
