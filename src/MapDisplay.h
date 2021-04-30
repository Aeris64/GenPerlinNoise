#ifndef MAPDISPLAY_H
#define MAPDISPLAY_H

#include <SFML/Graphics.hpp>

class MapDisplay
{
public:
	static sf::Uint8* DrawNoiseMap(float* noiseMap, const int width, const int height, sf::Uint8* pixels)
	{
		// sf::Color* colourMap = new sf::Color[width * height];

		// Traverse the 2D array
		for (auto y = 0; y < height; y++)
		{
			for (auto x = 0; x < width; x++)
			{
				const float value = *(noiseMap + y * width + x);

				// *(colourMap + y * width + x) = MapDisplay::LerpRGB(sf::Color::Black, sf::Color::White, value);
				// MapDisplay::WriteColor(pixels, x, y, width, MapDisplay::LerpRGB(sf::Color::Black, sf::Color::White, value));
				sf::Color myColor = sf::Color::Black;
				if (value > 0.5) myColor = sf::Color::White;
				MapDisplay::WriteColor(pixels, x, y, width, myColor);
			}
		}

		//// Traverse the 2D array
		//for (auto y = 0; y < height; y++)
		//{
		//	for (auto x = 0; x < width; x++)
		//	{
		//		MapDisplay::WriteColor(pixels, x, y, width, *(colourMap + y * width + x));
		//		auto t = *(colourMap + y * width + x);
		//		// std::cout << t.r << ", " << t.g << ", " << t.b << std::endl;
		//	}
		//}

		return pixels;
	}

	static sf::Color LerpRGB(sf::Color a, sf::Color b, float v)
	{
		return sf::Color
		(
			a.r + (b.r - a.r) * v,
			a.g + (b.g - a.g) * v,
			a.b + (b.b - a.b) * v,
			a.a + (b.a - a.a) * v
		);
	}

	// Fonction qui permet de "vider" l'array, ici j'initialise tous les pixels à (255, 0, 0, 255), donc rouge sans transparence.
	static void EmptyArray(sf::Uint8* pixels, int width, int height)
	{
		for (int j = 0; j < height; ++j)
		{
			for (int i = 0; i < width; ++i)
			{
				pixels[4 * (j * width + i)] = 255;
				pixels[4 * (j * width + i) + 1] = 0;
				pixels[4 * (j * width + i) + 2] = 0;
				pixels[4 * (j * width + i) + 3] = 255;
			}
		}
	}

	// Fonction qui permet de change un pixel, en lui affectant une couleur RGB, on lui laisse sa transparence à 255 (donc sans transparence).
	static void WriteColor(sf::Uint8* pixels, int x, int y, int width, sf::Color color)
	{
		pixels[4 * (y * width + x)] = color.r;
		pixels[4 * (y * width + x) + 1] = color.g;
		pixels[4 * (y * width + x) + 2] = color.b;
		pixels[4 * (y * width + x) + 3] = 255;
	}
};

#endif // !MAPDISPLAY_H