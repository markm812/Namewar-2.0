#include "loading.h"

loading::loading(int& s, const std::string &path, SDL_Window *window, SDL_Renderer* renderer)
{
	delay = rand() % 5;
	stage = s;
	init(path,window,renderer);
}

loading::~loading()
{
}



void loading::init(const std::string &path, SDL_Window *window, SDL_Renderer *renderer)
{
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) std::cerr <<"Failed to init png image" <<IMG_GetError() << std::endl;
	SDL_Surface* loaded_Surface = SDL_LoadBMP(path.c_str());
	if (!loaded_Surface) std::cerr << "Failed to load loadingScreen png" << IMG_GetError() <<std::endl;
	screen_Texture = SDL_CreateTextureFromSurface(renderer,loaded_Surface);
	SDL_FreeSurface(loaded_Surface);
	src_loading_icon.x = 194;
	src_loading_icon.y = 28;
	src_loading_icon.h = 556;
	src_loading_icon.w = 586;
	desr_loading_icon.w = 300;
	desr_loading_icon.h = 300;
	returnDelay();
}

void loading::Display(SDL_Renderer *renderer, int xpos, int ypos)
{
	desr_loading_icon.x = xpos;
	desr_loading_icon.y = ypos;
	SDL_RenderCopy(renderer, screen_Texture, &src_loading_icon, &desr_loading_icon);
}

void loading::fetch()
{
}

int loading::returnStage()
{
	return stage;
}

int loading::returnWidth()
{
	return desr_loading_icon.w;
}

int loading::returnDelay()
{
	return delay;
}