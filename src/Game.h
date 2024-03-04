#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <string>
#include <SDL_thread.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_net.h"
#include "ggponet.h"

// Forward declaration of ThreadData struct
struct ThreadData;

class Game
{
public:
    // Constructor and destructor
    Game();
    ~Game();

    // Public member functions
    void startGameLoop();

private:
    // Private member variables
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Surface* m_surface;
    SDL_Texture* m_texture;
    SDL_Event* m_event;
    Mix_Music* m_sound;
    UDPsocket m_socket;
    IPaddress m_SenderIPaddress;
    IPaddress m_RecieverIPaddress;
    Uint64 m_frameStart;
    Uint64 m_frameTime;
    Uint64 m_frameCount;
    float m_avgFPS;
    Uint64 m_fpsTimer;
    Uint64 m_fpsCount;
    bool showFrameRate;
    std::string temp;

    // Private member functions
    void init();
    void cleanup();
    void update();
    void render();
    void getFrames();
    void readInput();
    void playAudio();
    void sendPacket(const std::string& message);
    void receivePacket();
    void StartSendThread();
    void StartReceiveThread();
    static int SendThreadFunction(void* arg);
    static int ReceiveThreadFunction(void* arg);
};

// Declaration of ThreadData struct
struct ThreadData {
    Game* gameInstance;
    const char* message;
};

constexpr Uint16 SCREEN_HEIGHT = 480;
constexpr Uint16 SCREEN_WIDTH = 640;
constexpr Uint16 BUFFER_SIZE = 1024;

#endif // GAME_H
