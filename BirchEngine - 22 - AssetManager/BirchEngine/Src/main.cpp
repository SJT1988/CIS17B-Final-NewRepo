#include "Game.h"

Game *game = nullptr;

int main(int argc, char *argv[])
{

	const int FPS = 60;
	const int FRAMEDELAY = 1000 / FPS; // max time between frames
	Uint32 frameStart;
	int frameTime;

	game = new Game();
	game->init("GameWindow", 352, 352, false);

	while (game->running())
	{
		frameStart = SDL_GetTicks(); //frames elapsed by this moment
		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart; //(roughly) time in ms taken by a single frame
		if (FRAMEDELAY > frameTime) {
			SDL_Delay(FRAMEDELAY - frameTime);
		}
	}

	game->clean();
	return 0;
}