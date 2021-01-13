#ifndef player_h
#define player_h
#endif // !player_h

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

#include "Text_c.h"
#pragma once
class player
{
public:
	player* other = NULL;
	player(std::string nameStr, Uint16 nameInput[], SDL_Renderer* rend);
	~player();

	std::string name;
	Uint16 uintName[1024];
	int max_hp;

	// setup functions and return functions
	void rollState();
	bool alive();
	void printOutName(SDL_Renderer* render);
	double returnStats(int which);
	std::string returnName();

	// functions for attack events
	int attack(int round);
	int takeDamage(int dmg, int flag,int round);
	int normalAttack(int dmg,int round);
	int rollSpell(int flag, int round);

	// functions for handling spell events
	void cursed();
	void takePill();
	void charmed();
	void goldenDong();
	void rockShield();
	void sickDodge();
	void bondge();
	void lucky();
	void smited();
private:
	/*
	ID	stats
	00	health;
	01	strength;
	02	defence;
	03	agility;
	04	luck;
	05	sanity;
	*/
	double stats[6] = { 232,46,5,8,5,0 };
	SDL_Renderer* render = nullptr;
};

