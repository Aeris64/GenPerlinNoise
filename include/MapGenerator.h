#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "CustomNoise.h"
#include "MapDisplay.h"

class MapGenerator
{
public:
	float* noiseMapFinal;

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

private:
	void PixelCheck(const float valueCheck, const int limitY);

	bool PixelAllCheck(const int x, const int y, const float valueCheck, const int limitY);
	bool PixelTopCheck(int x, int y, const float valueCheck, const int limitY);
	bool PixelLeftCheck(int x, int y, const float valueCheck, const int limitY);
	bool PixelRightCheck(int x, int y, const float valueCheck, const int limitY);
	bool PixelBottomCheck(int x, int y, const float valueCheck, const int limitY);

	//void OnDraw(std::unique_ptr<sf::RenderWindow> m_pWindow)
	//{
	//	// On écrit notre image à l'écran.
	//	m_pWindow->draw(sprite);
	//}
};

#endif // !MAPGENERATOR_H
