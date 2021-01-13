#include "loadingDot.h"

loadingDot::loadingDot(int s, const std::string &path, SDL_Renderer *renderer)
{
	duration = s;
	init(path, renderer);
}

void loadingDot::init(const std::string &path, SDL_Renderer *renderer)
{
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (!loadedSurface) std::cerr << "Failed to load surface bitmpa" << std::endl;
	load_text_texture = load_dot_texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);

	src_text_rect.x = 26;
	src_text_rect.y = 1202;
	src_text_rect.h = 276;
	src_text_rect.w = 728;
	pos_text_rect.w = 250;
	pos_text_rect.h = 90;

	src_dot_rect.w = 300;
	src_dot_rect.h = 50;
	src_dot_rect.x = 50;
	pos_dot_rect.w = 150;
	pos_dot_rect.h = 30;

}

void loadingDot::display_dot(int xpos, int ypos, SDL_Renderer* renderer, int duration)
{
	int buffer = 100;
	src_dot_rect.y = 723 + duration*buffer;
	std::cout << src_dot_rect.y << std::endl;
	pos_dot_rect.x = xpos;
	pos_dot_rect.y = ypos;
	std::cout << "Updated new dot" << std::endl;
	SDL_RenderCopy(renderer,load_text_texture,&src_dot_rect,&pos_dot_rect);
}

void loadingDot::display_text(int xpos, int ypos, SDL_Renderer* renderer)
{
	pos_text_rect.x = xpos;
	pos_text_rect.y = ypos;
	SDL_RenderCopy(renderer,load_text_texture,&src_text_rect,&pos_text_rect);
}

int loadingDot::return_dot_width()
{
	return pos_dot_rect.w;
}

int loadingDot::return_text_width()
{
	return pos_text_rect.w;
}

int loadingDot::return_text_height()
{
	return pos_text_rect.h;
}


loadingDot::~loadingDot()
{
}
