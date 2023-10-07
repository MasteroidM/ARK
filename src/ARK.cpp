#include "ARK.h"

int main(int argc, char* args[])
{
	Status status = CreateWindow();

	if (status == SUCCESS)
		std::cout << "Success" << std::endl;
	else
		std::cout << "Failure" << std::endl;

<<<<<<< HEAD
    if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG) {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;

    }

    SDL_Surface* image = IMG_Load("src/images/640.jpg");
    if (image == nullptr) {
        std::cout << "Unable to load image 640.jpg! SDL_image Error: " << IMG_GetError() << std::endl;

    }

    SDL_Renderer* renderer = SDL_CreateRenderer(sdlWindow.get(), -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;

    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    if (texture == nullptr) {
        std::cout << "Unable to create texture from 640.jpg! SDL Error: " << SDL_GetError() << std::endl;

    }

    while (!quit) {
        while (SDL_PollEvent(&event)) {
           

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, nullptr, nullptr);
            SDL_RenderPresent(renderer);

           
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }
    }
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);


    SDL_Quit();

    return 0;
=======
	return 0;
>>>>>>> 7aa5f3e16a19364049e7815296ff9436aaa2500a
}