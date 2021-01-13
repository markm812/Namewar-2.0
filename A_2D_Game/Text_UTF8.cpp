#include "Text_UTF8.h"

Text_UTF8::Text_UTF8(const std::string& font_path, int font_size, char* msg, const SDL_Color& color, SDL_Renderer* renderer, int outline_size)
{
	_text_Texture = loadFont(font_path, font_size, msg, color, renderer, outline_size);
	SDL_QueryTexture(_text_Texture, nullptr, nullptr, &text_Rect->w, &text_Rect->h);
}

Text_UTF8::~Text_UTF8()
{
}

void Text_UTF8::Display(int xpos, int ypos, SDL_Renderer* renderer) const
{
	text_Rect->x = xpos;
	text_Rect->y = ypos;
	SDL_RenderCopy(renderer, _text_Texture, nullptr, text_Rect);
}

SDL_Texture* Text_UTF8::loadFont(const std::string& font_path, int font_size, char* message, const SDL_Color& color, SDL_Renderer* renderer, int outline_size)
{
	TTF_Font* font_outline = TTF_OpenFont(font_path.c_str(), font_size);

	TTF_SetFontOutline(font_outline, outline_size);
	
	auto text_surface_outline = TTF_RenderUTF8_Blended(font_outline,localeToUTF8(message),color);

	auto text_Texture = SDL_CreateTextureFromSurface(renderer, text_surface_outline);
	if (!text_Texture) std::cerr << "Failed to create texture from text surface" << std::endl;

	return text_Texture;
}

int Text_UTF8::return_Rect_width()
{
	return text_Rect->w;
}

int Text_UTF8::return_Rect_height()
{
	return text_Rect->h;
}

char* Text_UTF8::localeToUTF8(char* src)
{
	static char* buf = NULL;
	if (buf) 
	{
		free(buf);
		buf = NULL;
	}
	wchar_t* unicode_buf;
	int nRetLen = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
	unicode_buf = (wchar_t*)malloc((nRetLen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, src, -1, unicode_buf, nRetLen);
	nRetLen = WideCharToMultiByte(CP_UTF8, 0, unicode_buf, -1, NULL, 0, NULL, NULL);
	buf = (char*)malloc(nRetLen + 1);
	WideCharToMultiByte(CP_UTF8, 0, unicode_buf, -1, buf, nRetLen, NULL, NULL);
	free(unicode_buf);
	return buf;
}

