#include "Drawer.h"

Drawer::Drawer()
{
	Drawer::quit = false;
 
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("SDL2 Pixel Drawing",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

	renderer = SDL_CreateRenderer(window, -1, 0);
}

void Drawer::PlacePixel(Color color, Point point)
{
	SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);
	SDL_RenderDrawPoint(renderer, point.X, point.Y);
}

void Drawer::Present()
{
	SDL_RenderPresent(renderer);
}

void Drawer::WaitForUser()
{
	while (!quit)
    {
        SDL_WaitEvent(&event);
 
        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
			case SDL_KEYDOWN:
			{
				SDL_Keycode key = event.key.keysym.sym;
				if (key == SDLK_ESCAPE || 
					key == SDLK_KP_ENTER)
				{
					quit = true;
				}
			}
				break;
			case SDL_MOUSEBUTTONDOWN:
			{
				Uint8 btn = event.button.button;
				if (btn == SDL_BUTTON_LEFT ||
					btn == SDL_BUTTON_MIDDLE ||
					btn == SDL_BUTTON_RIGHT)
				{
					quit = true;
				}
			}
				break;
        }
    }
 
    SDL_DestroyWindow(window);
    SDL_Quit();
}
