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
#include <stringapiset.h>


class Text_UTF8
{
public:
	Text_UTF8(const std::string& font_path, int font_size, char* msg, const SDL_Color& color, SDL_Renderer* renderer, int outline_size);
	~Text_UTF8();

	void Display(int xpos, int ypos, SDL_Renderer* renderer) const;
	SDL_Texture* loadFont(const std::string& font_path, int font_size,char* message, const SDL_Color& color, SDL_Renderer* renderer, int outline_size);

	int return_Rect_width();
	int return_Rect_height();
	char* localeToUTF8(char* src);
private:
	SDL_Texture* _text_Texture = nullptr;
	SDL_Rect *text_Rect = new SDL_Rect;
};

