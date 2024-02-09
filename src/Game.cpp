#include "Game.h"

bool quit = false;

std::string temp="null";
Uint16 SenderPORT;
Uint16 RecieversPORT;

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

    if (m_sound)
        Mix_FreeMusic(m_sound);

    if (m_socket)
        SDLNet_UDP_Close(m_socket);

    SDLNet_Quit();
    Mix_CloseAudio();
    SDL_Quit();

}

void Game::startGameLoop()
{
    init();
    playAudio();

    while (!quit)
    {
        update();
        //std::cout << "This data will be sent in this tick" << SDL_GetKeyName(m_event->key.keysym.sym) << std::endl;

        Game::sendPacket(temp);        
        Game::receivePacket();
    }
}

void Game::init()
{
    m_event = new SDL_Event;

    //take senders and reciever ports as input
    std::cout << "Enter a port number (This port is your port where you will recieve data.)" << std::endl;
    std::cin >> SenderPORT;

    std::cout << "Enter a port number (This port is recievers port where you will send data.)" << std::endl;
    std::cin >> RecieversPORT;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
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

    if (SDLNet_Init() == -1) {
        std::cout << "SDLNet initialization failed: " << SDLNet_GetError() << std::endl;
    }

    m_socket = SDLNet_UDP_Open(SenderPORT);

    if (!m_socket) {
        std::cout << "SDLNet_UDP_Open failed: " << SDLNet_GetError() << std::endl;
    }

    if (SDLNet_ResolveHost(&m_SenderIPaddress, "192.168.43.199", SenderPORT) == -1) {
        std::cout << "SDLNet_ResolveHost failed: " << SDLNet_GetError() << std::endl;
    }

    if (SDLNet_ResolveHost(&m_RecieverIPaddress, "192.168.43.169", RecieversPORT) == -1) {
        std::cout << "SDLNet_ResolveHost failed: " << SDLNet_GetError() << std::endl;
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


    if ((showFrameRate == true))
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
    if (m_event->type == SDL_SCANCODE_P) playAudio();
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
        temp = SDL_GetKeyName(m_event->key.keysym.sym);
        std::cout << "Key pressed: " << temp << std::endl;
        switch (m_event->key.keysym.sym)
        {
        case SDLK_p:
            if (Mix_PausedMusic()) Mix_ResumeMusic();
            else Mix_PauseMusic();
            break;

        case SDLK_o:
            Mix_HaltMusic();
            break;
        }
        break;

    case SDL_KEYUP:
        temp = SDL_GetKeyName(m_event->key.keysym.sym);
        std::cout << "Key released: " << temp  << std::endl;
        break;

    default:
        temp = "null";
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

void Game::playAudio()
{
    Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC | MIX_INIT_MOD);

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096)) {
        printf("Unable to open audio!\n");
        SDL_Quit();
        exit(1);
    }

    m_sound = Mix_LoadMUS("audio.flac");
    std::cout << Mix_GetMusicVolume(m_sound) << std::endl;
    if (m_sound == nullptr) SDL_ShowSimpleMessageBox(NULL, "", "Music is null", m_window);
    Mix_PlayMusic(m_sound, -1);
    if (!Mix_PlayingMusic()) SDL_ShowSimpleMessageBox(NULL, "", "No music playing", m_window);

}

void Game::sendPacket(const std::string& message) {

    char buffer[BUFFER_SIZE];
    strcpy_s(buffer, BUFFER_SIZE, message.c_str());

    UDPpacket* packet = SDLNet_AllocPacket(BUFFER_SIZE);
    packet->address.host = m_RecieverIPaddress.host;
    packet->address.port = m_RecieverIPaddress.port;
    packet->data = (Uint8*)(buffer);
    packet->len = strlen(buffer) + 1;

    if (SDLNet_UDP_Send(m_socket, -1, packet) == 0) {
        std::cout << "SDLNet_UDP_Send failed: " << SDLNet_GetError() << std::endl;
    }

    SDLNet_FreePacket(packet);
}

void Game::receivePacket() {
    char buffer[BUFFER_SIZE];

    UDPpacket* packet = SDLNet_AllocPacket(BUFFER_SIZE);

    if (SDLNet_UDP_Recv(m_socket, packet)) {
        std::cout << "Received packet from " << SDLNet_ResolveIP(&packet->address) << ": " << packet->data << std::endl;
    }

    SDLNet_FreePacket(packet);
}


