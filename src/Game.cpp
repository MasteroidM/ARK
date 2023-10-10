#include "Game.h"

bool quit = false;

Game::Game() = default;

Game::~Game()
{
    if (m_window)
        SDL_DestroyWindow(m_window);

    if (m_renderer)
        SDL_DestroyRenderer(m_renderer);

    if (m_surface)
        SDL_FreeSurface(m_surface);

    if (m_texture)
        SDL_DestroyTexture(m_texture);

    if (m_event)
        delete m_event;

    SDL_Quit();

}

void Game::startGameLoop()
{
    initVideo();
    while (!quit)
    {
        update();
    }
}

void Game::initVideo()
{
    m_event = new SDL_Event;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL initialization failed. SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    if (IMG_Init(IMG_INIT_JPG & IMG_INIT_JPG) != IMG_INIT_JPG)
    {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
    }

    m_window = SDL_CreateWindow("ARK", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (m_window == nullptr) {
        std::cout << "Window creation failed. SDL Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    m_surface = IMG_Load("images/640.jpg");
    if (m_surface == nullptr)
    {
        std::cout << "Unable to load image 640.jpg! SDL_image Error: " << IMG_GetError() << std::endl;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr) {
        std::cout << "Renderer creation failed. SDL Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        exit(1);
    }


    m_texture = SDL_CreateTextureFromSurface(m_renderer, m_surface);
    if (m_texture == nullptr)
    {
        std::cout << "Unable to create texture from 640.jpg! SDL Error: " << SDL_GetError() << std::endl;
    }

}

void Game::update()
{
    m_frameStart = SDL_GetTicks64();

    while (SDL_PollEvent(m_event))
    {
        readInput();
    }

    render();

}

void Game::render()
{
    //SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    //SDL_RenderClear(m_renderer);
    //SDL_RenderPresent(m_renderer);

    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);

    m_frameTime = SDL_GetTicks64() - m_frameStart;

    const Uint64 targetFrameTime = 1000 / 60;
    if (m_frameTime < targetFrameTime)
    {
        SDL_Delay(targetFrameTime - m_frameTime);
    }

    m_fpsCount++;


    if ((showFrameRate == true) || (_DEBUG))
    {
        getFrames();
    }
    else
    {
        m_fpsCount = 0;
        m_fpsTimer = SDL_GetTicks();
    }

}

void Game::readInput()
{
    switch (m_event->type)
    {
    case SDL_QUIT:
        quit = true;
        break;

    case SDL_MOUSEBUTTONDOWN:
        if (m_event->button.button == SDL_BUTTON_LEFT)
            std::cout << "Mouse Pressed : Left Mouse Button (LMB)" << std::endl;
        if (m_event->button.button == SDL_BUTTON_RIGHT)
            std::cout << "Mouse Pressed : Right Mouse Button (RMB)" << std::endl;
        if (m_event->button.button == SDL_BUTTON_MIDDLE)
            std::cout << "Mouse Pressed : Middle Mouse Button (MMB)" << std::endl;
        break;

    case SDL_MOUSEBUTTONUP:
        if (m_event->button.button == SDL_BUTTON_LEFT)
            std::cout << "Mouse Released : Left Mouse Button (LMB)" << std::endl;
        if (m_event->button.button == SDL_BUTTON_RIGHT)
            std::cout << "Mouse Released : Right Mouse Button (RMB)" << std::endl;
        if (m_event->button.button == SDL_BUTTON_MIDDLE)
            std::cout << "Mouse Released : Middle Mouse Button (MMB)" << std::endl;
        break;

    case SDL_MOUSEWHEEL:
        if (m_event->wheel.y > 0)
            std::cout << "Mouse Wheel Up" << std::endl;
        if (m_event->wheel.y < 0)
            std::cout << "Mouse Wheel Down" << std::endl;
        break;

    case SDL_KEYDOWN:
        std::cout << "Key pressed: " << SDL_GetKeyName(m_event->key.keysym.sym) << std::endl;
        break;

    case SDL_KEYUP:
        std::cout << "Key released: " << SDL_GetKeyName(m_event->key.keysym.sym) << std::endl;
        break;

    default:
        break;

    }
}

void Game::getFrames()
{
    if (SDL_GetTicks() - m_fpsTimer >= 1000) 
    {
        m_avgFPS = m_fpsCount / ((SDL_GetTicks64() - m_fpsTimer) / 1000.0f);
        std::cout << "Current FPS: " << m_avgFPS << std::endl;
        m_fpsCount = 0;
        m_fpsTimer = SDL_GetTicks();
    }
}

