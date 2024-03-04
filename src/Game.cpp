#include "Game.h"

bool quit = false;
Uint16 SenderPORT;
Uint16 RecieversPORT;

Game::Game() :
    m_window(nullptr), m_renderer(nullptr), m_surface(nullptr), m_texture(nullptr), m_event(nullptr),
    m_sound(nullptr), m_socket(nullptr), m_frameStart(0), m_frameTime(0), m_frameCount(0),
    m_avgFPS(0.0f), m_fpsTimer(0), m_fpsCount(0), showFrameRate(true), temp("null") {}

Game::~Game()
{
    if (m_event) {
        delete m_event;
        m_event = nullptr;
    }

    cleanup();
}

void Game::startGameLoop()
{
    init();
    playAudio();
    SDL_Thread* sendThread = nullptr;
    SDL_Thread* receiveThread = nullptr;

    while (!quit)
    {
        update();
        

        if (sendThread) {
            SDL_WaitThread(sendThread, nullptr);
            sendThread = nullptr;
        }
        if (receiveThread) {
            SDL_WaitThread(receiveThread, nullptr);
            receiveThread = nullptr;
        }

        StartSendThread();
        StartReceiveThread();
    }
}

void Game::init()
{
    m_event = new SDL_Event;

    std::cout << "Enter a port number (This port is your port where you will recieve data.)" << std::endl;
    std::cin >> SenderPORT;

    std::cout << "Enter a port number (This port is recievers port where you will send data.)" << std::endl;
    std::cin >> RecieversPORT;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "SDL initialization failed. SDL Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Initialize SDL_image
    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    m_window = SDL_CreateWindow("ARK", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (m_window == nullptr) {
        std::cout << "Window creation failed. SDL Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }
    // Load image
    m_surface = IMG_Load("images/640.jpg");
    if (m_surface == nullptr)
    {
        std::cerr << "Unable to load image 640.jpg! SDL_image Error: " << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
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

    // Initialize SDL_mixer
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096)) {
        std::cerr << "Unable to open audio: " << Mix_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Load music
    m_sound = Mix_LoadMUS("audio.flac");
    if (m_sound == nullptr) {
        std::cerr << "Unable to load music: " << Mix_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Initialize SDL_net
    if (SDLNet_Init() == -1) {
        std::cerr << "SDLNet initialization failed: " << SDLNet_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Open UDP socket
    m_socket = SDLNet_UDP_Open(SenderPORT);
    if (!m_socket) {
        std::cerr << "SDLNet_UDP_Open failed: " << SDLNet_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Resolve IP addresses
    if (SDLNet_ResolveHost(&m_SenderIPaddress, "127.0.0.1", SenderPORT) == -1) {
        std::cerr << "SDLNet_ResolveHost failed for sender: " << SDLNet_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if (SDLNet_ResolveHost(&m_RecieverIPaddress, "127.0.0.1", RecieversPORT) == -1) {
        std::cerr << "SDLNet_ResolveHost failed for receiver: " << SDLNet_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Game::cleanup()
{
    // Cleanup SDL_net
    SDLNet_Quit();

    // Cleanup SDL_mixer
    Mix_CloseAudio();

    // Cleanup SDL_image
    IMG_Quit();

    // Cleanup SDL
    SDL_Quit();
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
    // Rendering code...
    Uint64 frameStart = SDL_GetTicks64();

    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);

    Uint64 frameTime = SDL_GetTicks64() - frameStart;

    const Uint64 targetFrameTime = 1000 / 60;
    if (frameTime < targetFrameTime)
    {
        SDL_Delay(targetFrameTime - frameTime);
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

void Game::getFrames()
{
    // Frame rate calculation...
    if (SDL_GetTicks() - m_fpsTimer >= 1000)
    {
        m_avgFPS = m_fpsCount / ((SDL_GetTicks64() - m_fpsTimer) / 1000.0f);
        std::cout << "Current FPS: " << m_avgFPS << std::endl;
        m_fpsCount = 0;
        m_fpsTimer = SDL_GetTicks();
    }
}

void Game::readInput()
{
    // Input handling code...
    if (m_event->type == SDL_KEYDOWN)
    {
        temp = SDL_GetKeyName(m_event->key.keysym.sym);
        std::cout << "Key pressed: " << temp << std::endl;
        switch (m_event->key.keysym.sym)
        {
        case SDLK_p:
            if (Mix_PausedMusic())
                Mix_ResumeMusic();
            else
                Mix_PauseMusic();
            break;

        case SDLK_o:
            Mix_HaltMusic();
            break;
        }
    }
    else if (m_event->type == SDL_KEYUP)
    {
        temp = SDL_GetKeyName(m_event->key.keysym.sym);
        std::cout << "Key released: " << temp << std::endl;
    }
    else if (m_event->type == SDL_MOUSEBUTTONDOWN)
    {
        if (m_event->button.button == SDL_BUTTON_LEFT)
            std::cout << "Mouse Pressed : Left Mouse Button (LMB)" << std::endl;
        if (m_event->button.button == SDL_BUTTON_RIGHT)
            std::cout << "Mouse Pressed : Right Mouse Button (RMB)" << std::endl;
        if (m_event->button.button == SDL_BUTTON_MIDDLE)
            std::cout << "Mouse Pressed : Middle Mouse Button (MMB)" << std::endl;
    }
    else if (m_event->type == SDL_MOUSEBUTTONUP)
    {
        if (m_event->button.button == SDL_BUTTON_LEFT)
            std::cout << "Mouse Released : Left Mouse Button (LMB)" << std::endl;
        if (m_event->button.button == SDL_BUTTON_RIGHT)
            std::cout << "Mouse Released : Right Mouse Button (RMB)" << std::endl;
        if (m_event->button.button == SDL_BUTTON_MIDDLE)
            std::cout << "Mouse Released : Middle Mouse Button (MMB)" << std::endl;
    }
    else if (m_event->type == SDL_MOUSEWHEEL)
    {
        if (m_event->wheel.y > 0)
            std::cout << "Mouse Wheel Up" << std::endl;
        if (m_event->wheel.y < 0)
            std::cout << "Mouse Wheel Down" << std::endl;
    }
    else if (m_event->type == SDL_QUIT)
    {
        quit = true;
    }
    else
    {
        temp = "null";
    }
}

void Game::playAudio()
{
    // Audio playback code...
    if (!Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC | MIX_INIT_MOD)) {
        printf("Unable to initialize SDL_mixer: %s\n", Mix_GetError());
        // Handle initialization failure
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        printf("Unable to open audio: %s\n", Mix_GetError());
        // Handle audio opening failure
    }

    m_sound = Mix_LoadMUS("audio.flac");
    if (!m_sound) {
        printf("Unable to load music: %s\n", Mix_GetError());
        // Handle music loading failure
    }

    std::cout << Mix_GetMusicVolume(m_sound) << std::endl;
    Mix_PlayMusic(m_sound, -1);
    if (!Mix_PlayingMusic()) {
        printf("No music playing: %s\n", Mix_GetError());
        // Handle music playing failure
    }
}

void Game::sendPacket(const std::string& message)
{
    // Sending packet code...
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
    memset(buffer, 0, BUFFER_SIZE);
}

void Game::receivePacket()
{
    // Receiving packet code...
    char buffer[BUFFER_SIZE];

    UDPpacket* packet = SDLNet_AllocPacket(BUFFER_SIZE);

    if (SDLNet_UDP_Recv(m_socket, packet)) {
        std::cout << "Received packet from " << SDLNet_ResolveIP(&packet->address) << ": " << packet->data << std::endl;
    }

    SDLNet_FreePacket(packet);
}

void Game::StartSendThread()
{
    // Start sending thread...
    // Allocate a ThreadData structure on the heap
    ThreadData* data = new ThreadData;
    data->gameInstance = this; // Assign the current Game instance
    data->message = temp.c_str(); // Assign the message string

    // Start the thread, passing the address of the ThreadData structure
    SDL_Thread* thread = SDL_CreateThread(Game::SendThreadFunction, "ThreadName", data);
    // Handle thread creation result...
    if (thread == nullptr) {
        // Handle thread creation failure
        delete data; // Clean up the allocated ThreadData structure
    }
}

void Game::StartReceiveThread()
{
    // Start receiving thread...
    // Allocate a ThreadData structure on the heap
    ThreadData* data = new ThreadData;
    data->gameInstance = this; // Assign the current Game instance
    data->message = nullptr; // No message to pass for receiving packets

    // Start the thread, passing the address of the ThreadData structure
    SDL_Thread* thread = SDL_CreateThread(Game::ReceiveThreadFunction, "ReceiveThreadName", data);
    // Handle thread creation result...
    if (thread == nullptr) {
        // Handle thread creation failure
        delete data; // Clean up the allocated ThreadData structure
    }
}

int Game::SendThreadFunction(void* arg)
{
    // Sending thread function...
    // Cast the void* to a struct that contains both the Game instance and the string data
    ThreadData* data = static_cast<ThreadData*>(arg);
    // Now you can use the Game instance and the message string inside your thread function
    data->gameInstance->sendPacket(data->message);
    // Cleanup the allocated ThreadData structure
    delete data;
    return 0;
}

int Game::ReceiveThreadFunction(void* arg)
{
    // Receiving thread function...
    ThreadData* data = static_cast<ThreadData*>(arg);
    data->gameInstance->receivePacket();
    delete data;

    return 0;
}
