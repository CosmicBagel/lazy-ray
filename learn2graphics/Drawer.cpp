#include "Drawer.h"
#include "fmt/format.h"

Drawer::Drawer(int width, int height)
{ 
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("SDL2 Pixel Drawing",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

	renderer = SDL_CreateRenderer(window, -1, 0);
	buffer = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);
	
}

//Implement faster pixel drawing
//http://gigi.nullneuron.net/gigilabs/sdl2-pixel-drawing/
void Drawer::PlacePixel(Color color, Point point)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(renderer, point.x, point.y);
}

void Drawer::PlacePixels(Color color, Point* points, int count)
 {
	// SDL_RenderDrawPoints(renderer, points, count);
	//SDL_Point p[] = {{4,5}, {54,3}};

	SDL_RenderDrawPoints(renderer, points, count);
	//LogError(fmt::format("SDL Error: {}", SDL_GetError()));
}

void Drawer::PlacePixels(Color color, std::vector<Point> points)
{
	int size = points.size();
	SDL_RenderDrawPoints(renderer, points.data(), size);
}

void Drawer::Present()
{
	SDL_RenderPresent(renderer);
}

void Drawer::WaitForUser()
{
	bool quit = false;
	bool user_input = false;

	while (!quit && !user_input)
    {
        SDL_WaitEvent(&event);

		SDL_Keycode key = event.key.keysym.sym;
		Uint8 btn = event.button.button;
 
        switch (event.type)
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
        SDL_WaitEvent(&event);

		SDL_Keycode key = event.key.keysym.sym;
 
        switch (event.type)
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
	SDL_DestroyWindow(window);
    SDL_Quit();
}

void Drawer::LogInfo(std::string message)
{
	SDL_LogInfo(0, message.c_str());
}

void Drawer::LogError(std::string message)
{
	SDL_LogError(0, message.c_str());
}
