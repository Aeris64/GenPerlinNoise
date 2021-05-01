#ifndef MAPDISPLAY_H
#define MAPDISPLAY_H

#include <SFML/Graphics.hpp>

class MapDisplay
{
public:
	static sf::Uint8* DrawNoiseMap(float* noiseMap, const int width, const int height, sf::Uint8* pixels);

	static sf::Color LerpRGB(sf::Color a, sf::Color b, float v);

	// Fonction qui permet de "vider" l'array, ici j'initialise tous les pixels à (255, 0, 0, 255), donc rouge sans transparence.
	static void EmptyArray(sf::Uint8* pixels, int width, int height);

	// Fonction qui permet de change un pixel, en lui affectant une couleur RGB, on lui laisse sa transparence à 255 (donc sans transparence).
	static void WriteColor(sf::Uint8* pixels, int x, int y, int width, sf::Color color);
};

#endif // !MAPDISPLAY_H