#pragma once

#include <iostream>
#include <memory>

#include "SDL.h"

const uint16_t SCREEN_HEIGHT = 480;
const uint16_t SCREEN_WIDTH = 640;

class Window {
public:
    Window();
    ~Window();

    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> CreateWindow();

private:
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window{ nullptr, &SDL_DestroyWindow };
    SDL_Surface* screenSurface{ nullptr };
};
