#include "Text_c.h"

Text_c::Text_c(const std::string& font_path, int font_size,const Uint16 msg[], const SDL_Color& color, SDL_Renderer* renderer, int outline_size)
{
	_text_Texture = loadFont(font_path, font_size, msg, color, renderer, outline_size);
	SDL_QueryTexture(_text_Texture, nullptr, nullptr, &text_Rect.w, &text_Rect.h);

}

Text_c::~Text_c()
{
}

void Text_c::Display(int xpos, int ypos, SDL_Renderer* renderer) const
{
	text_Rect.x = xpos;
	text_Rect.y = ypos;
	SDL_RenderCopy(renderer, _text_Texture, nullptr, &text_Rect);
}

SDL_Texture* Text_c::loadFont(const std::string& font_path, int font_size, const Uint16 msg[], const SDL_Color& color, SDL_Renderer* renderer, int outline_size)
{
	TTF_Font* font = TTF_OpenFont(font_path.c_str(), font_size);
	TTF_Font* font_outline = TTF_OpenFont(font_path.c_str(), font_size);
	//std::cout << font_path << std::endl;
	TTF_SetFontOutline(font_outline, outline_size);
	if (!font) std::cerr << "Failed to load font!" << std::endl;

	auto text_surface_outline = TTF_RenderUNICODE_Solid(font_outline, msg, {255,255,255,255});

	auto text_surface = TTF_RenderUNICODE_Solid(font,msg, color);
	if (!text_surface) std::cerr << "Failed to create text surface!" << std::endl;

	auto text_Texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	if (!text_Texture) std::cerr << "Failed to create texture from text surface" << std::endl;

	SDL_FreeSurface(text_surface);
	return text_Texture;
}

int Text_c::return_Rect_width()
{
	return text_Rect.w;
}

int Text_c::return_Rect_height()
{
	return text_Rect.h;
}
