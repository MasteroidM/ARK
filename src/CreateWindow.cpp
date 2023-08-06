#include "CreateWindow.h"

void KeepWindowUp()
{
	SDL_Event e;
	bool quit = false;
	while (quit == false)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
	}
}

Status CreateWindow()
{
	SDL_Window* window = nullptr;
	SDL_Surface* screenSurface = nullptr;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! " << "SDL Error : " << SDL_GetError() << std::endl;
	}
	else
	{
		window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (window == nullptr)
		{
			std::cout << "SDL Window could not be created! " << "SDL Error : " << SDL_GetError() << std::endl;
		}
		else
		{
			screenSurface = SDL_GetWindowSurface(window);

			SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));

			SDL_UpdateWindowSurface(window);

			KeepWindowUp();
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return SUCCESS;
}