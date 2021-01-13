#include "Game.h"
#include "Text.h"
#include "loading.h"
#include "loadingDot.h"
#include "Text_c.h"
#include "player.h"
#include "Text_UTF8.h"

#pragma warning(disable : 4996)

Game::Game()
{
}

Game::~Game()
{
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	w = width;
	h = height;
	int flags = 0;
	if (fullscreen) { flags = SDL_WINDOW_FULLSCREEN; }

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Initialized Successfully!" << std::endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		if (window)
		{
			std::cout << "Window initialzed and created!" << std::endl;
			SDL_Surface* iSurface = SDL_LoadBMP("Texture/G_icon.bmp");
			if (iSurface == NULL) std::cerr << "Failed to load game icon image" << std::endl;
			SDL_SetWindowIcon(window,iSurface);
			SDL_FreeSurface(iSurface);
			
		} 
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_Surface* loadedSurface = IMG_Load("Texture/mainscreen.png");
			SDL_Texture* gTexture = SDL_CreateTextureFromSurface(renderer,loadedSurface);
			if (gTexture == NULL) std::cout << "Failed to load texture" << std::endl;
			else std::cout << "gTexture loaded successfully!" << std::endl;
			SDL_RenderCopy(renderer, gTexture, NULL, NULL);
			std::cout << "Renderer created succsesfully!" << std::endl;
			SDL_RenderPresent(renderer);

			SDL_free(loadedSurface);
			//SDL_free(gTexture);
		}
		if (TTF_Init() == -1) std::cout << "Failed to init SDL2_ttf" << std::endl;
		Text text_out("Fonts/arial.ttf", 30, "Press Enter to Start", {0,0,0,255},renderer,4);
		Text text_inner("Fonts/arial.ttf", 30, "Press Enter to Start", { 255,255,255,255 }, renderer,0);
		text_out.Display(width / 2 - text_out.return_Rect_width() / 2, height / 5 * 4 - 3, renderer);
		text_inner.Display(width / 2 - text_inner.return_Rect_width() / 2, height / 5 * 4, renderer);

		Uint16 msg[1024] = {26449,38263,25794,21488};
		Text_c title_out("Fonts/kaiu.ttf", 108,msg, {255,255,255,225},renderer,8);
		Text_c title_inner("Fonts/kaiu.ttf", 108,msg , { 0,0,0,225},renderer,0);
		title_out.Display(width / 2 - title_out.return_Rect_width() / 2, height / 4 * 1, renderer);
		title_inner.Display(width / 2 - title_inner.return_Rect_width() / 2+2, height / 4 * 1 , renderer);
		render();
		isRunning = true;
	}
	else
	{
		isRunning = false;
	}
}

void Game::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym) 
			{
				case SDLK_RETURN:
					if (stage == 0)
						stage += 1;
					break;
				default:
					break;
			}
			break;
		}
	default:
		break;
	}
}

void Game::update()
{
	if (stage == 1) 
		loadingStage();
	if (stage == 2)
		pickName();
	if (stage == 3)
	{
		isRunning = false;
		SDL_Delay(30000);
	}
}

void Game::render()
{
	SDL_ShowWindow(window);
	if (renderer == nullptr) std::cerr << "Renderer not found, access violation may be triggered!" << std::endl;
	SDL_RenderPresent(renderer);
	//SDL_RenderClear(renderer);
	system("cls");
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_Quit();
	SDL_QUIT;
	std::cout << "Game clean, quitting." << std::endl;
}

SDL_Texture* Game::loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		std::cout << ("Unable to load image %s! SDL_image Error: %s\n", path.c_str());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			std::cout << ("Unable to create texture from %s! SDL Error: %s\n", path.c_str());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

void Game::loadingStage()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	loading* loadin= new loading(stage, "Texture/loadingSc.bmp", window, renderer);
	loadin->Display(renderer,w/2 - loadin->returnWidth()/2, h/6);
	if (loadin->returnStage() != 1) std::cout << "Stage: " << loadin->returnStage() << std::endl;
	loadingDot* dot_text = new loadingDot(stage,"Texture/loadingSc.bmp",renderer);
	dot_text->display_text(w/2 - dot_text->return_text_width()/2-100, 5*h/6, renderer);
	render();

	int delay = loadin->returnDelay();
	std::cout << "delay:" << delay << std::endl;
	int n = 0;
	for (int i = 0; i < delay; i++)
	{
		dot_text->display_dot(w/2 - dot_text->return_text_width()/2+150,5*h/6+dot_text->return_text_height()/3,renderer,n%3);
		n++;
		render();
		Sleep(500);
	}

	stage += 1;
	std::cout << stage << std::endl;
	delete loadin;
	delete dot_text;
}

void Game::pickName()
{
	system("cls");
	SDL_RenderClear(renderer);
	SDL_Texture* outputTexture = loadTexture("Texture/waiting.png");
	SDL_RenderCopy(renderer, outputTexture, NULL, NULL);
	render();
	std::string name1;
	Uint16 name1Uint16[1024];
	std::cout << "玩家1名字: \n";
	std::cin >> name1;
	for (int i = 0; i < name1.length(); i++)
		name1Uint16[i] = name1[i];
	p1 = new player(name1,name1Uint16,renderer);
	std::string name2;
	Uint16 name2Uint16[1024];
	std::cout << "玩家2名字: \n";
	std::cin >> name2;
	for (int i = 0; i < name2.length(); i++)
		name2Uint16[i] = name2[i];
	p2 = new player(name2,name2Uint16,renderer);

	SDL_DestroyTexture(outputTexture);
	SDL_ShowWindow(window);
	Sleep(500);
	start();
}

void Game::start()
{

	p1->other = p2;
	p2->other = p1;

	Sleep(500);
	//SDL_RenderClear(renderer);
	f2fCompare();

	SDL_Delay(3000);

	SDL_RenderClear(renderer);
	stageSetUp();
	updateHp();

	player* list[2] = { p1,p2 };
	int ptr;
	int round = 0;
	//SDL_Renderer* backUp = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	p1->returnStats(3) > p2->returnStats(3) ? ptr = 0 : ptr = 1;
	while (p1->alive() && p2->alive())
	{
		SDL_Delay(1500);
		//handleEvents();
		round = list[ptr%2]->attack(round);
		// reset round
		if (round >= 20)
		{
			SDL_Delay(1000);
			SDL_RenderClear(renderer);
			stageSetUp();
			round = 0;
		}
		updateHp();
		SDL_Delay(1000);
		ptr++;
	}
	if (p1->alive())
	{
		std::string msg = p2->returnName() + "輸了";
		Text_UTF8* opS = new Text_UTF8("Fonts/kaiu.ttf", 20, (char*)msg.c_str(), {0,0,0,255},renderer,0);
		opS->Display(390, 170 + 25 * (round), renderer);
		render();
		delete opS;
	}
	else
	{
		std::string msg = p1->returnName() + "輸了";
		Text_UTF8* opS = new Text_UTF8("Fonts/kaiu.ttf", 20, (char*)msg.c_str(), { 0,0,0,255 }, renderer, 0);
		opS->Display(390, 170 + 25 * (round), renderer);
		render();
		delete opS;
	}
	stage++;
}

void Game::stageSetUp()
{
	SDL_Color Black = { 0,0,0,255 };
	SDL_Texture* showingTexture = loadTexture("Texture/battleStage.png");
	Text_UTF8 p1Name("Fonts/kaiu.ttf", 18, ((char*)p1->returnName().c_str()), Black, renderer, 0);
	Text_UTF8 p2Name("Fonts/kaiu.ttf", 18, ((char*)p2->returnName().c_str()), Black, renderer, 0);

	SDL_RenderClear(renderer);
	SDL_ShowWindow(window);
	SDL_RenderCopy(renderer, showingTexture, NULL, NULL);

	p1Name.Display(600-p1Name.return_Rect_width(),120-p1Name.return_Rect_height(),renderer);
	p2Name.Display(645, 120 - p2Name.return_Rect_height(), renderer);

	SDL_DestroyTexture(showingTexture);
}

void Game::f2fCompare()
{
	SDL_Texture* outputTexture = loadTexture("Texture/statsCompare.png");

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, outputTexture, NULL, NULL);
	p1->printOutName(renderer);
	render();
	SDL_DestroyTexture(outputTexture);
}

void Game::updateHp()
{
	SDL_Color Black = { 0,0,0,255 };

	// hp bar rest
	SDL_Texture* resetBar1 = loadTexture("Texture/greenBar.png");
	SDL_Texture* resetBar2 = loadTexture("Texture/greenBar.png");
	if (resetBar1 == NULL || resetBar2 == NULL)
		std::cerr << "failed to reset green bar.\n";
	SDL_Rect* pos_rectr1 = new SDL_Rect;
	SDL_Rect* pos_rectr2 = new SDL_Rect;

	pos_rectr1->x = 145;
	pos_rectr1->y = 33;
	pos_rectr1->w = 463;
	pos_rectr1->h = 53;

	pos_rectr2->x = 638;
	pos_rectr2->y = 33;
	pos_rectr2->w = 463;
	pos_rectr2->h = 53;

	// %hp left
	double percent1;
	double percent2;
	percent1 = ( (double)(p1->max_hp) - (double)(p1->returnStats(0)) )/ (double)(p1->max_hp);
	if (percent1 > 1) percent1 = 1;
	percent2 = ((double)(p2->max_hp) - (double)(p2->returnStats(0))) / (double)(p2->max_hp);
	if (percent2 > 1) percent2 = 1;

	// current hp bar texture
	SDL_Texture* outputTexture1 = loadTexture("Texture/bar.png");
	SDL_Texture* outputTexture2 = loadTexture("Texture/bar.png");

	// current hp text
	Text p1HpNumb("Fonts/arial.ttf", 24, std::to_string((int)p1->returnStats(0)), Black, renderer, 0);
	Text p2HpNumb("Fonts/arial.ttf", 24, std::to_string((int)p2->returnStats(0)), Black, renderer, 0);

	// max hp number text
	//std::string oText = "/" + std::to_string(p1->max_hp);
	//Text p1Hp("Fonts/arial.ttf", 24, oText, Black, renderer, 0);
	//oText = std::to_string(p2->max_hp) + "/";
	//Text p2Hp("Fonts/arial.ttf", 24, oText, Black, renderer, 0);

	// hp bar position
	SDL_Rect* src_rect1 = new SDL_Rect;
	SDL_Rect* pos_rect1 = new SDL_Rect;
	SDL_Rect* pos_rect2 = new SDL_Rect;
	src_rect1->x = 0;
	src_rect1->y = 0;
	src_rect1->h = 59;
	src_rect1->w = 463;

	pos_rect1->x = 145;
	pos_rect1->y = 33;
	pos_rect1->w = percent1 * (double)463;;
	pos_rect1->h = 53;

	pos_rect2->w = percent2 * (double)463;;
	pos_rect2->h = 53;
	pos_rect2->x = 1101 - pos_rect2->w;
	pos_rect2->y = 33;

	SDL_RenderCopy(renderer, resetBar1, src_rect1, pos_rectr1);
	SDL_RenderCopy(renderer, resetBar2, src_rect1, pos_rectr2);

	SDL_RenderCopy(renderer, outputTexture1, src_rect1, pos_rect1);
	SDL_RenderCopy(renderer, outputTexture2, src_rect1, pos_rect2);

	//p1Hp.Display(550, 48, renderer);
	//p2Hp.Display(650, 48, renderer);

	p1HpNumb.Display(550 , 48, renderer);
	p2HpNumb.Display(650 , 48, renderer);

	render();
	//SDL_free(outputTexture1);
	//SDL_free(outputTexture2);	
	delete src_rect1;
	delete pos_rect1;
	delete pos_rect2;
	delete pos_rectr1;
	delete pos_rectr2;
	SDL_DestroyTexture(resetBar1);
	SDL_DestroyTexture(resetBar2);
	SDL_DestroyTexture(outputTexture1);
	SDL_DestroyTexture(outputTexture2);
}





