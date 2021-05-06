#ifndef CUSTOMNOISE_H
#define CUSTOMNOISE_H

class CustomNoise
{
public:
	float* noises;

	CustomNoise(const int mapWidth, const int mapHeight, float scale, const int octaves, const float persistance, const float lacunarity, const int seed);
	~CustomNoise() = default;

	float* GenerateNoise();

	float Lerp(const float a, const float b, const float t);
	float InvLerp(const float a, const float b, const float t);

	void SetWidth(int newWidth);
	void SetHeight(int newHeight);
	void SetScale(float newScale);
	void SetSeed(int newSeed);
	void SetOctaves(int newOctaves);
	void SetPersistance(float newPersistance);
	void SetLacunarity(float newLacunarity);

private:
	int mapWidth;
	int mapHeight;
	float scale;
	int seed;

	int octaves;
	float persistance;
	float lacunarity;
};

#endif // !CUSTOMNOISE_H