#include "CreateWindow.h"

Window::Window() = default;

Window::~Window() {
	SDL_Quit();
}

std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> Window::CreateWindow() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! " << "SDL Error : " << SDL_GetError() << std::endl;
        return std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>(nullptr, SDL_DestroyWindow);
    }

    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> windowPtr(SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN), SDL_DestroyWindow);

    if (windowPtr == nullptr) {
        std::cout << "SDL Window could not be created! " << "SDL Error : " << SDL_GetError() << std::endl;
        return std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>(nullptr, SDL_DestroyWindow);
    }

    screenSurface = SDL_GetWindowSurface(windowPtr.get());

    SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));

    SDL_UpdateWindowSurface(windowPtr.get());

    return windowPtr;
}
