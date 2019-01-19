#include "Drawer.h"
#include "fmt/format.h"

//Constructor for Drawer, takes dimensions for the canvas
Drawer::Drawer(int width, int height)
{ 
	//canvas dimensions
	width_ = width;
	height_ = height;

	// we draw all of our pixels to a cpu buffer first
	// then copy that to a buffer in the gpu so that it can be displayed
	bufferCPU_ = new Uint32[width_ * height_];
	pixelPitch_ = width_ * sizeof(Uint32);

	//set initial cpu buffer color to black
	memset(bufferCPU_, 0, width_ * height_ * sizeof(Uint32));

	//Only intend on use SDL for video and logs, this
	//is just a graphical learning project
    SDL_Init(SDL_INIT_VIDEO);

    window_ = SDL_CreateWindow("SDL2 Pixel Drawing",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_, height_, 0);

	renderer_ = SDL_CreateRenderer(window_, -1, 0);

	//Important to note the pixel format as our CPU buffer needs to match that
	//Endianness could also be a factor here
	//Texture is target since we plan on rendering to it (see SDL documentation)
	bufferGPU_ = SDL_CreateTexture(renderer_,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, width_, height_);
}

// place a pixel to the 
void Drawer::PlacePixel(Color color, Point point)
{
	//bounds check
	if (point.x >= width_ || point.y >= height_)
	{
		LogError(fmt::format(
			"Attempted to place pixel out of bounds: {0}, {1}", point.x, point.y));
		return;
	}
	//texure format is ARGB 8888
	//might have to work on making this faster, can't directly cast the struct
	//so have to do a little bit manipulation thingy
	//ARGB, but we start on the left hand side of the bits
	Uint32 pixel = color.b | (color.g << 8) | (color.r << 16) | (color.a << 24);
	bufferCPU_[point.y * width_ + point.x] = pixel;
}

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
	SDL_RenderCopy(renderer_, bufferGPU_, NULL, NULL);
	SDL_RenderPresent(renderer_);
}

//Wait for the user to press literally any button or key
//If the user hits escape or closes the window, the program still responds
//by closing down
void Drawer::WaitForUser()
{
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


// clean up our stuff and close
void Drawer::Close()
{
	delete[] bufferCPU_;

	SDL_DestroyTexture(bufferGPU_);
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
    SDL_Quit();
}

//Log information using normal c++ strings, fmt::format is also
//very useful in conjunction with this.
//message: standard c++ string
void Drawer::LogInfo(std::string message)
{
	SDL_LogInfo(0, message.c_str());
}

//Like info log, but will show up with ERROR in front of it
//Only use for critical errors
//message: standard c++ string
void Drawer::LogError(std::string message)
{
	SDL_LogError(0, message.c_str());
}
