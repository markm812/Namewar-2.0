#ifndef loading_H
#define loading_H

#pragma once

#include "Game.h"

class loading
{
public:
	loading(int& s, const std::string &path, SDL_Window *window, SDL_Renderer* renderer);
	~loading();
	void init(const std::string &path, SDL_Window *window, SDL_Renderer* renderer);
	void Display(SDL_Renderer * renderer, int xpos, int ypos);
	void fetch();
	int returnStage();
	int returnWidth();
	int returnDelay();
private:
	int stage;
	int delay;
	SDL_Texture* screen_Texture = nullptr;
	SDL_Rect src_loading_icon;
	SDL_Rect desr_loading_icon;
};
#endif // !loading_H
