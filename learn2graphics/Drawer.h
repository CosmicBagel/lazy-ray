#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>

using Point = SDL_Point;
using Color = SDL_Color;

class Drawer
{
private:
	bool closed;
	SDL_Renderer * renderer_;
	SDL_Event event_;
	SDL_Window * window_;
	SDL_Texture * bufferGPU_;
	//rgba:8+8+8+8=32
	Uint32 * bufferCPU_;
	int pixelPitch_;
	int width_;
	int height_;
	int bufferSize_;

	unsigned int g_seed;
public:
	Drawer(int width, int height);
	void PlacePixel(Color color, Point point);
	void PlacePixel(Color color, int bufferIndex);
	void PlacePixelQuad(Color colors[], Point points[]);
	void Present();
	void WaitToClose();
	void CheckForClose();
	void WaitForUser();
	void Close();
	bool IsClosed();
	void CheckForSdlError();

	void LogInfo(std::string message);
	void LogError(std::string message);

	void SeedRandomGenerator(int seed);
	int GetRandom(int max);

	static void RGBtoHSL(const Color& color, Uint32& h, Uint32& s, Uint32& l);

};