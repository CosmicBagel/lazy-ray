#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "fmt/format.h"
#include <time.h>

using Point = SDL_Point;
 // using Color = SDL_Color;

union Color
{
	struct
	{
		Uint8 b;
	    Uint8 g;
	    Uint8 r;
	    Uint8 a;
	};
	//we can cast this into a single int, which is how it will be stored
	//in the texture and blited to the screen
	uint32_t color_int;
};

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

	int bufferWriteOffset_;

	unsigned int g_seed;
public:
	Drawer(const int width, const int height, const char* windowTitle);

	void PlacePixel(Color const& color, Point const& point)
	{
		PlacePixel(color, point.y * width_ + point.x);
	}
	void PlacePixel(Color const& color, int const& bufferIndex)
	{
		//bounds check
		if (bufferIndex > bufferSize_ - 1)
		{
			LogError(fmt::format(
				"Attempted to place pixel out of bounds, buffer index: {}", bufferIndex));
			return;
		}

		//texure format is ARGB 8888
		//might have to work on making this faster, can't directly cast the struct
		//so have to do a little bit manipulation thingy
		//ARGB, but we start on the left hand side of the bits
		
		bufferCPU_[bufferIndex] = color.b | (color.g << 8) | (color.r << 16) | (color.a << 24);
	}

	//should run 19,200 times a frame
	void PlacePixelBatch(Uint32 const* batch, int const& batchCount)
	{
		const int batchSize = sizeof(Uint32) * batchCount; //64 bytes
		Uint32 * p = bufferCPU_ + bufferWriteOffset_;
		//when doing pointer arithmetic, each addition is in fact four bytes
		//eg p + 1, advances the pointer by four bytes, note one byte
		memcpy(p, batch, batchSize);

		bufferWriteOffset_ += batchCount;
		if (bufferWriteOffset_ >= bufferSize_)
			bufferWriteOffset_ = 0;

	}

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