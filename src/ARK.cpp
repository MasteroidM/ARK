#include "ARK.h"

int main(int, char**) 
{
    Window window;
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> sdlWindow = window.CreateWindow(); 

    SDL_Event event;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }
    }

    SDL_Quit();

    return 0;
}