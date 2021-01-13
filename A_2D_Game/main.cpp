#include "Game.h"
#include <time.h>

Game* game = nullptr;

int main(int agrc, char* agrs[]) {
	srand(time(NULL));
	game = new Game();
	game->init("ADT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
	while (game->running()) {
		game->handleEvents();
		game->update();
		game->render();
	}
	game->clean();
	return 0;
}