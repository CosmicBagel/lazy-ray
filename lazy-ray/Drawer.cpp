#include "Drawer.h"
#include "fmt/format.h"
#include <time.h>

//todo separate logging into its own class

//todo research SIMD, (and other single thread things that go fast)
//todo put fast random into its own class
//todo implement faster color to unit32 conversion
//todo implement faster cpu buffer writing

//todo implement frame time tracker (ms prolly)
//todo implement just enough gui stuff to display mean, median, and mode frame time stats
//todo implement frame time jitter tracking

//todo implement frame breakdown chart (section for each part of the render)

//Constructor for Drawer, takes dimensions for the canvas
Drawer::Drawer(const int width, const int height, const char* windowTitle)
{ 
	closed = false;

	//canvas dimensions
	width_ = width;
	height_ = height;
	bufferSize_ = width * height;
	bufferWriteOffset_ = 0;

	// we draw all of our pixels to a cpu buffer first
	// then copy that to a buffer in the gpu so that it can be displayed
	bufferCPU_ = new Uint32[bufferSize_];
	pixelPitch_ = width_ * sizeof(Uint32);

	//set initial cpu buffer color to black
	memset(bufferCPU_, 0, width_ * height_ * sizeof(Uint32));

	//Only intend on use SDL for video and logs, this
	//is just a graphical learning project
    SDL_Init(SDL_INIT_VIDEO);

    window_ = SDL_CreateWindow(windowTitle,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_, height_, 0);

	renderer_ = SDL_CreateRenderer(window_, -1, 0);

	//Important to note the pixel format as our CPU buffer needs to match that
	//Endianness could also be a factor here
	//Texture is target since we plan on rendering to it (see SDL documentation)
	bufferGPU_ = SDL_CreateTexture(renderer_,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, width_, height_);

	//just in case there was an error while initializing
	CheckForSdlError();
}

// place a pixel to the 
// inline void Drawer::PlacePixel(Color const& color, Point const& point)
// {	
// 	PlacePixel(color, point.y * width_ + point.x);
// }

// inline void Drawer::PlacePixel(Color const& color, int const& bufferIndex)
// {
// 	//bounds check
// 	if (bufferIndex > bufferSize_ - 1)
// 	{
// 		LogError(fmt::format(
// 			"Attempted to place pixel out of bounds, buffer index: {}", bufferIndex));
// 		return;
// 	}
//
// 	//texure format is ARGB 8888
// 	//might have to work on making this faster, can't directly cast the struct
// 	//so have to do a little bit manipulation thingy
// 	//ARGB, but we start on the left hand side of the bits
// 	Uint32 pixel = color.b | (color.g << 8) | (color.r << 16) | (color.a << 24);
// 	bufferCPU_[bufferIndex] = pixel;
// }


// Place four pixels at a time, since it seems like processors
// like to do things in groups of fours. Might axe this function
// colors: Array of exactly 4 colors (stack mem, not heap)
// points: Array of exactly 4 points (stack mem, not heap)
void Drawer::PlacePixelQuad(Color colors[], Point points[])
{
	//do a memset for the four pixels in the pixels_ array
}

// Take CPU buffer, write it into GPU memory.
// Then we update the on screen image with the updated GPU buffer
void Drawer::Present()
{
	SDL_UpdateTexture(bufferGPU_, NULL, bufferCPU_, pixelPitch_);

	SDL_RenderClear(renderer_);
	SDL_RenderCopyEx(renderer_, bufferGPU_, 
		NULL, NULL, NULL, NULL, SDL_FLIP_VERTICAL);
	SDL_RenderPresent(renderer_);

	//reset cpu buffer to black
	memset(bufferCPU_, 0, width_ * height_ * sizeof(Uint32));

	//just in case there was an error while rendering this last frame
	CheckForSdlError();
}

//Wait for the user to press literally any button or key
//If the user hits escape or closes the window, the program still responds
//by closing down
void Drawer::WaitForUser()
{
	if (closed)
		return;

	//clear event queue before initiating wait
	while (SDL_PollEvent(&event_) != 0) {}

	bool quit = false;
	bool user_input = false;

	while (!quit && !user_input)
    {
        SDL_WaitEvent(&event_);

		SDL_Keycode key = event_.key.keysym.sym;
		Uint8 btn = event_.button.button;
 
        switch (event_.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
			case SDL_KEYDOWN:
				user_input = true;
				if (key == SDLK_ESCAPE)
					quit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				user_input = true;
				// if (btn == SDL_BUTTON_LEFT)
				// {
				// 	quit = true;
				// }
				break;
        }
    }

	if (quit) Close();
}

//Wait for user to hit escape or close the window, then close
void Drawer::WaitToClose()
{
	if (closed)
		return;

	bool quit = false;

	while (!quit)
    {
        SDL_WaitEvent(&event_);

		SDL_Keycode key = event_.key.keysym.sym;
 
        switch (event_.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
			case SDL_KEYDOWN:
				if (key == SDLK_ESCAPE)
					quit = true;
				break;
        }
    }

    Close();
}

void Drawer::CheckForClose()
{
	//go through event queue and check for close events
	//throw away other events
	while (SDL_PollEvent(&event_) != 0)
	{
		if (event_.type == SDL_QUIT)
		{
			Close();
			return;
		}
	}
}

bool Drawer::IsClosed()
{
	return closed;
}

// clean up our stuff and close
void Drawer::Close()
{
	if (!closed)
	{	
		delete[] bufferCPU_;

		SDL_DestroyTexture(bufferGPU_);
		SDL_DestroyRenderer(renderer_);
		SDL_DestroyWindow(window_);
	    SDL_Quit();
		closed = true;
	}
}

void Drawer::CheckForSdlError()
{
	//sdl pretty much exclusively uses c style strings so
	//it's a bit trickier to use
	auto e = SDL_GetError();
	//strlen is the way to check the length of a c style string
	if (strlen(e) != 0 )
	{
		LogError(fmt::format("SDL_Error: {}", e));
		SDL_ClearError();
	}
}

//Log information using normal c++ strings, fmt::format is also
//very useful in conjunction with this.
//message: standard c++ string
void Drawer::LogInfo(std::string message)
{
	SDL_LogInfo(0, message.c_str());
}

//Like info log, but will show up with ERROR in front of it
//Only use for critical errors, often I'll leave a breakpoint on this
//So I can take a look at the call stack and what not
//message: standard c++ string
void Drawer::LogError(std::string message)
{
	SDL_LogError(0, message.c_str());
}

//ripped from intel's site
//Used to seed the generator.
void Drawer::SeedRandomGenerator( int seed )
{
	g_seed = seed;
}

//fastrand routine returns one integer, similar output value range as C lib.
//does not go below 0
//max: int exclusive roof
int Drawer::GetRandom(int max)
{
	g_seed = (214013*g_seed+2531011);
	return ((g_seed>>16)&0x7FFF) % max;
}

void Drawer::RGBtoHSL(const Color& color, Uint32& h, Uint32& s, Uint32& l)
{
	Uint32 r = color.r;
	Uint32 g = color.g;
	Uint32 b = color.b;

	double r_percent = ((double)r)/255;
	double g_percent = ((double)g)/255;
	double b_percent = ((double)b)/255;

	double max_color = 0;
	if((r_percent >= g_percent) && (r_percent >= b_percent))
		max_color = r_percent;
	if((g_percent >= r_percent) && (g_percent >= b_percent))
		max_color = g_percent;
	if((b_percent >= r_percent) && (b_percent >= g_percent))
		max_color = b_percent;

	double min_color = 0;
	if((r_percent <= g_percent) && (r_percent <= b_percent))
		min_color = r_percent;
	if((g_percent <= r_percent) && (g_percent <= b_percent))
		min_color = g_percent;
	if((b_percent <= r_percent) && (b_percent <= g_percent))
		min_color = b_percent;

	double L = 0;
	double S = 0;
	double H = 0;

	L = (max_color + min_color)/2;

	if(max_color == min_color)
	{
		S = 0;
		H = 0;
	}
	else
	{
		if(L < .50)
		{
			S = (max_color - min_color)/(max_color + min_color);
		}
		else
		{
			S = (max_color - min_color)/(2 - max_color - min_color);
		}
		if(max_color == r_percent)
		{
			H = (g_percent - b_percent)/(max_color - min_color);
		}
		if(max_color == g_percent)
		{
			H = 2 + (b_percent - r_percent)/(max_color - min_color);
		}
		if(max_color == b_percent)
		{
			H = 4 + (r_percent - g_percent)/(max_color - min_color);
		}
	}
	s = (Uint32)(S*100);
	l = (Uint32)(L*100);
	H = H*60;
	if(H < 0)
		H += 360;
	h = (Uint32)H;
}