#ifndef TEXT_C_H
#define TEXT_C_H
#endif // !TEXT_C_H

#pragma once
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
#include <time.h>
class Text_c
{
public:
	Text_c(const std::string& font_path, int font_size, const Uint16 msg[], const SDL_Color& color, SDL_Renderer* renderer, int outline_size);
	~Text_c();

	void Display(int xpos, int ypos, SDL_Renderer* renderer) const;
	static SDL_Texture* loadFont(const std::string& font_path, int font_size, const Uint16 msg[], const SDL_Color& color, SDL_Renderer* renderer, int outline_size);

	int return_Rect_width();
	int return_Rect_height();

private:
	SDL_Texture* _text_Texture = nullptr;
	mutable SDL_Rect text_Rect;
};

