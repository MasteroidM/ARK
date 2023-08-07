#pragma once

#include <iostream>

#include "SDL.h"

constexpr uint16_t SCREEN_HEIGHT = 480;
constexpr uint16_t SCREEN_WIDTH = 640;

enum Status { SUCCESS, FAILURE };

Status CreateWindow();
void KeepWindowUp();
