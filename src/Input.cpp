
#include "Input.h"


while (SDL_PollEvent(&Event)) {
    if (Event.type == SDL_KEYDOWN) {
        std::cout
            << "Key Pressed! Key Code: "
            << Event.key.keysym.sym
            << ", Key Name: "
            << SDL_GetKeyName(Event.key.keysym.sym)
            << '\n';
    }
    else if (
        Event.type == SDL_KEYDOWN &&
        Event.key.keysym.sym == SDLK_SPACE
        ) {
        int x, y;
        Uint32 Buttons{ SDL_GetMouseState(&x, &y) };

        std::cout << "Mouse is at " << x << ", " << y;
        if ((Buttons & SDL_BUTTON_LMASK)) {
            std::cout << " - Left Button is pressed";
        }
        if ((Buttons & SDL_BUTTON_RMASK)) {
            std::cout << " - Right Button is pressed";
        }
        std::cout << "\n";
    }
}