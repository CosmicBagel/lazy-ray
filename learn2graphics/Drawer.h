#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>

using Point = SDL_Point;
using Color = SDL_Color;

// struct Point
// {
// 	int X;
// 	int Y;
// };

// struct Color
// {
// 	Uint8 R;
// 	Uint8 G;
// 	Uint8 B;
// 	Uint8 A;
// };

class Drawer
{
private:
	SDL_Renderer * renderer_;
	SDL_Event event_;
	SDL_Window * window_;
	SDL_Texture * buffer_;
	//rgba:8+8+8+8=32
	Uint32 * pixels_;
	int pixelPitch_;
	int width_;
	int height_;
public:
	Drawer(int width, int height);
	void PlacePixel(Color color, Point point);
	void PlacePixels(Color color, std::vector<Point> points);
	void PlacePixelQuad(Color colors[], Point points[]);
	void PlacePixels(Color color, Point* points, int count);
	void Present();
	void WaitToClose();
	void WaitForUser();
	void Close();

	void LogInfo(std::string message);
	void LogError(std::string message);

};