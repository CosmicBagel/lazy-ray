#include "Drawer.h"
#include "fmt/format.h"

Drawer::Drawer(int width, int height)
{ 
	width_ = width;
	height_ = height;

	pixels_ = new Uint32[width_ * height_];
	pixelPitch_ = width_ * sizeof(Uint32);

    SDL_Init(SDL_INIT_VIDEO);

    window_ = SDL_CreateWindow("SDL2 Pixel Drawing",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_, height_, 0);

	renderer_ = SDL_CreateRenderer(window_, -1, 0);
	buffer_ = SDL_CreateTexture(renderer_,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width_, height_);

	//set inital pixel color to black
	memset(pixels_, 0, width_ * height_ * sizeof(Uint32));
}

void Drawer::PlacePixel(Color color, Point point)
{
	//bounds check
	if (point.x >= width_ || point.y >= height_)
	{
		LogError(fmt::format(
			"Attempted to place pixel out of bounds: {0}, {1}", point.x, point.y));
		return;
	}
	//texure format is ARBG 8888
	//might have to work on making this faster
	Uint32 pixel = color.b | (color.g << 8) | (color.r << 16) | (color.a << 24);
	pixels_[point.y * width_ + point.x] = pixel;
}

// Place four pixels at a time, since it seems like processors
// like to do things in groups of fours
// colors: Array of exactly 4 colors (stack mem, not heap)
// points: Array of exactly 4 points (stack mem, not heap)
void Drawer::PlacePixelQuad(Color colors[], Point points[])
{
	//do a memset for the four pixels in the pixels_ array
}

void Drawer::PlacePixels(Color color, Point* points, int count)
 {
	// SDL_RenderDrawPoints(renderer, points, count);
	//SDL_Point p[] = {{4,5}, {54,3}};

	SDL_RenderDrawPoints(renderer_, points, count);
	//LogError(fmt::format("SDL Error: {}", SDL_GetError()));
}

void Drawer::PlacePixels(Color color, std::vector<Point> points)
{
	int size = points.size();
	SDL_RenderDrawPoints(renderer_, points.data(), size);
}

void Drawer::Present()
{
	SDL_UpdateTexture(buffer_, NULL, pixels_, pixelPitch_);

	SDL_RenderClear(renderer_);
	SDL_RenderCopy(renderer_, buffer_, NULL, NULL);
	SDL_RenderPresent(renderer_);
}

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

void Drawer::Close()
{
	SDL_DestroyTexture(buffer_);
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
    SDL_Quit();

	delete[] pixels_;
}

void Drawer::LogInfo(std::string message)
{
	SDL_LogInfo(0, message.c_str());
}

void Drawer::LogError(std::string message)
{
	SDL_LogError(0, message.c_str());
}
