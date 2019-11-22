#pragma once

#include "SDL.h"
#include "SDL_Image.h"
#include <iostream>
#include <vector>

class AssetManager;
class ColliderComponent;

class Game
{
public:

	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	bool running() { return isRunning; }
	void render();
	void clean();

	static bool isRunning;
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static AssetManager* assets;
	enum groupLabels : std::size_t
	{
		groupMapBG,
		groupMap,
		groupMapFX,
		groupPlayers,
		groupColliders,
		groupProjectiles,
		groupMonsters
	};

private:
	
	//bool isRunning = false;
	int cnt = 0;
	SDL_Window *window;
};