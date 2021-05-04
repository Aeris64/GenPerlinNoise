#include "../include/MapDisplay.h"

sf::Uint8* MapDisplay::DrawNoiseMap(float* noiseMap, const int width, const int height, sf::Uint8* pixels)
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
			sf::Color myColor = sf::Color(128, 70, 27, 255);
			if (value == 0.25F) myColor = sf::Color(99, 102, 106, 255);
			if (value == 0.5F) myColor = sf::Color(255, 250, 250, 255);
			if (value == 0.75F) myColor = sf::Color(129, 165, 186, 255);
			if (value == 1.F) myColor = sf::Color(48, 128, 232, 255);
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

sf::Color MapDisplay::LerpRGB(sf::Color a, sf::Color b, float v)
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
void MapDisplay::EmptyArray(sf::Uint8* pixels, int width, int height)
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
void MapDisplay::WriteColor(sf::Uint8* pixels, int x, int y, int width, sf::Color color)
{
	pixels[4 * (y * width + x)] = color.r;
	pixels[4 * (y * width + x) + 1] = color.g;
	pixels[4 * (y * width + x) + 2] = color.b;
	pixels[4 * (y * width + x) + 3] = 255;
}