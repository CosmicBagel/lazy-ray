#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>

using Point = SDL_Point;
using Color = SDL_Color;

class Drawer
{
private:
	SDL_Renderer * renderer_;
	SDL_Event event_;
	SDL_Window * window_;
	SDL_Texture * bufferGPU_;
	//rgba:8+8+8+8=32
	Uint32 * bufferCPU_;
	int pixelPitch_;
	int width_;
	int height_;
public:
	Drawer(int width, int height);
	void PlacePixel(Color color, Point point);
	void PlacePixelQuad(Color colors[], Point points[]);
	void Present();
	void WaitToClose();
	void WaitForUser();
	void Close();
	void CheckForSdlError();

	void LogInfo(std::string message);
	void LogError(std::string message);

};