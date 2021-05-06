#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include <./SFML/Graphics.hpp>

class MapGenerator
{
public:
	MapGenerator(const int mapWidth, const int mapHeight, const float noiseScale, const int octaves, const float persistance, const float lacunarity, const int seed);

	void GenerateMap();
	sf::Sprite* GetSprite();

private:
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
};

#endif // !MAPGENERATOR_H
