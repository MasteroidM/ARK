#pragma once

#include <iostream>
#include <vector>
#include<string>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_net.h"
#include "ggponet.h"

class Game
{

public:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	SDL_Surface* m_surface;
	SDL_Texture* m_texture;
	SDL_Event* m_event;
	Mix_Music* m_sound;
	UDPsocket m_socket;
	IPaddress m_SenderIPaddress;
	IPaddress m_RecieverIPaddress;


	Uint64 m_frameStart = 0;
	Uint64 m_frameTime = 0;
	Uint64 m_frameCount = 0;
	float m_avgFPS = 0.0f;
	Uint64 m_fpsTimer = SDL_GetTicks64();
	Uint64 m_fpsCount = 0;
	bool showFrameRate = true;




	//public:
	Game();
	~Game();

	void startGameLoop();
	void update();
	void init();
	void render();
	void getFrames();
	void readInput();
	void playAudio();
	void sendPacket(const std::string& message);
	void receivePacket();
};

constexpr Uint16 SCREEN_HEIGHT = 480;
constexpr Uint16 SCREEN_WIDTH = 640;

constexpr Uint16 BUFFER_SIZE = 1024;

//constexpr Uint16 frameRate = 60;