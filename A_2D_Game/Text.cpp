#include "Text.h"

Text::Text(const std::string& font_path, int font_size, const std::string& message_text, const SDL_Color& color, SDL_Renderer* renderer, int outline_size)
{
		_text_Texture = loadFont(font_path, font_size, message_text, color, renderer, outline_size);
		SDL_QueryTexture(_text_Texture, nullptr, nullptr, &text_Rect.w, &text_Rect.h);
}

Text::~Text()
{
}

void Text::Display(int xpos, int ypos, SDL_Renderer* renderer) const
{
	text_Rect.x = xpos;
	text_Rect.y = ypos;
	SDL_RenderCopy(renderer,_text_Texture, nullptr, &text_Rect);
}

SDL_Texture* Text::loadFont(const std::string& font_path, int font_size, const std::string& message_text, const SDL_Color& color, SDL_Renderer *renderer, int outline_size)
{
		TTF_Font* font = TTF_OpenFont(font_path.c_str(), font_size);
		//std::cout << font_path << std::endl;
		TTF_SetFontOutline(font, outline_size);
		if (!font) std::cerr << "Failed to load font!" << std::endl;

		auto text_surface = TTF_RenderText_Solid(font, message_text.c_str(), color);
		if (!text_surface) std::cerr << "Failed to create text surface!" << std::endl;

		auto text_Texture = SDL_CreateTextureFromSurface(renderer, text_surface);
		if (!text_Texture) std::cerr << "Failed to create texture from text surface" << std::endl;

		SDL_FreeSurface(text_surface);
		return text_Texture;
}

int Text::return_Rect_width()
{
	return text_Rect.w;
}

int Text::return_Rect_height()
{
	return text_Rect.h;
}

