#ifndef Text_h
#define Text_h

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

class Text
{
public:
	// Constructor
	Text(const std::string &font_path, int font_size, const std::string &message_text, const SDL_Color &color, SDL_Renderer* renderer, int outline_size);
	~Text();
	// Rendering functions
	void Display(int xpos, int ypos, SDL_Renderer *renderer) const; 
	static SDL_Texture* loadFont(const std::string& font_path, int font_size, const std::string& message_text, const SDL_Color& color, SDL_Renderer *renderer, int outline_size);

	// Return value functions
	int return_Rect_width();
	int return_Rect_height();

	// chinese text
	//SDL_Texture* loadChinese(const std::string& font_path,int font_size, wchar_t msg[], const SDL_Color& color, SDL_Renderer* renderer, int outline_size);

private:
	SDL_Texture* _text_Texture = nullptr;
	mutable SDL_Rect text_Rect;
};
#endif // Text_h
