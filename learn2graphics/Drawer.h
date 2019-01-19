#pragma once
#include <SDL2/SDL.h>

struct Point
{
	int X;
	int Y;
};

struct Color
{
	Uint8 R;
	Uint8 G;
	Uint8 B;
	Uint8 A;
};

class Drawer
{
private:
	SDL_Renderer * renderer;
	bool quit;
	SDL_Event event;
	SDL_Window * window;
public:
	Drawer();
	void PlacePixel(Color color, Point point);
	void Present();
	void WaitForUser();
};