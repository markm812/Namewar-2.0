#ifndef loadingDot_h
#define loadingDot_h

#pragma once
#include "Game.h"
class loadingDot
{
public:
	loadingDot(int s, const std::string &path, SDL_Renderer *renderer);
	void init(const std::string &path, SDL_Renderer *renderer);
	void display_dot(int xpos, int ypos, SDL_Renderer * renderer, int duration);
	void display_text(int xpos, int ypos, SDL_Renderer* renderer);
	int return_dot_width();
	int return_text_width();
	int return_text_height();
	~loadingDot();
private:
	int duration;
	SDL_Texture* load_dot_texture;
	SDL_Texture* load_text_texture;
	SDL_Rect src_dot_rect;
	SDL_Rect pos_dot_rect;
	SDL_Rect src_text_rect;
	SDL_Rect pos_text_rect;
};
#endif // !loadingDot_h
