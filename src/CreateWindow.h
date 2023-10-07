#pragma once

#include <iostream>

#include "lib/include/SDL.h"
#include "lib/include/SDL_image.h"

constexpr uint16_t SCREEN_HEIGHT = 480;
constexpr uint16_t SCREEN_WIDTH = 640;

enum Status { SUCCESS, FAILURE };

Status CreateWindow();
void KeepWindowUp();
