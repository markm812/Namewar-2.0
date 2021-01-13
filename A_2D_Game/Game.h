#ifndef Game_h
#define Game_h

#pragma once
#pragma warning(suppress : 4996)

#include <iostream>
#include <string>
#include <SDL_ttf.h>
#include <SDL.h>
#include <windows.h>
#include <SDL_image.h>
#include <stack>
#include <vector>
#include <inttypes.h>
#include <math.h>

#include "player.h"
#include "Text.h"
#include "Text_c.h"
class Game
{
public:
	// Constructor & Destructor
	Game();
	~Game();
	// Initialize
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	// Game managing functions
	void handleEvents();
	void update();
	void render();
	void clean();

	SDL_Texture* loadTexture(std::string path);
	void loadingStage();
	bool running() { return isRunning;};
	void pickName();
	void start();
	void stageSetUp();
	void f2fCompare();
	void updateHp();
	SDL_Renderer* renderer = nullptr;
private:
	int w;
	int h;
	int stage = 0;
	bool isRunning = true;
	std::stack <int> game_state;
	SDL_Window *window = nullptr;
	player* p1;
	player* p2;
};
#endif // Game_h
