#include "player.h"
#include "Text.h"
#include "Text_UTF8.h"
#pragma warning(disable : 4996)
player::player(std::string nameStr, Uint16 nameInput[], SDL_Renderer* rend)
{
	for (int i = 0; i < nameStr.length(); i++)
		uintName[i] = nameInput[i];
	name = nameStr;
	render = rend;
	rollState();
}

void player::rollState()
{
	int focus = 0;
	double power = 0;
	double skillPoints[6] = { 0,0,0,0,0,0 };
	for (int i = 0; i < name.length(); i++)
	{
		// testing
		//std::cout << uintName[i] << " " << int(name[i%2]) << std::endl;
		power += (abs((int)(uintName[i])) + ((focus + name.length() - i + 1) * (i + 1)) * (i * i + 1)) % 163;
		focus += abs(int(name[i]) + 13) + power * 7;
	}
	power /= name.length();
	power < 75 ? power = (int)(75 + name.length() % 4) : power = power;

	focus = focus % 6;
	skillPoints[focus] += double(5) / double(12) * double(power);

	skillPoints[(focus + 1) % 6] += (double(2) / double(12)) * double(power);
	skillPoints[(focus + 5) % 6] += double(2) / double(12) * double(power);

	skillPoints[(focus + 2) % 6] += double(1) / double(12) * double(power);
	skillPoints[(focus + 4) % 6] += double(1) / double(12) * double(power);

	skillPoints[(focus + 3) % 6] += double(1) / double(12) * double(power);
	stats[0] += skillPoints[0] * 5.0;
	stats[1] += skillPoints[1] * 1.5;
	stats[2] += skillPoints[2] * 0.8;
	stats[3] += skillPoints[3] * 0.8;
	stats[4] += skillPoints[4] * 0.8;
	stats[5] += skillPoints[5] * 1.0;
	for (int i = 0; i < 6; i++)
		stats[i] = ceil(stats[i]);

	max_hp = stats[0];
}

bool player::alive()
{
	return (stats[0] > 0);
}

double player::returnStats(int which)
{
	if (this->stats[which] > 0) return this->stats[which];
	else return (int)0;
}

int player::attack(int round)
{
	int cast = 0;
	int crit = 0;
	int dmg = 0;

	srand((unsigned)time(0));
	rand() % 100 < 25+ (int)this->stats[5] ? cast = 1 : cast = 0;
	rand() % 100 < returnStats(4) ? crit = 1 : crit = 0;
	dmg = ((rand() % (int)(this->stats[1]*0.75)) + (int)(this->stats[1] * 0.25) )* (int)(1 + crit);

	if (dmg - other->returnStats(2) > other->returnStats(0))
		cast = 0;

	if (cast == 1)
		round = rollSpell(0, round);
	else
		round = normalAttack(dmg, round);

	// error exit code
	if (round < 0)
		return -1;

	return round;
}

// flag values: 
// 0 -> take normal attack
// 1 -> take counter attack
// 2 -> take skill damage
int player::takeDamage(int dmg, int flag, int round)
{
	int counterAttack;
	int rdUpdate = round;
	stats[0] -= dmg;
	if (flag == 0)
	{
		std::string outputString = "[攻擊]: " + other->name + " 攻向 " + name + " , 造成 " + std::to_string(dmg) + " 點傷害 ";
		Text_UTF8* opS = new Text_UTF8("Fonts/kaiu.ttf", 18, (char*)outputString.c_str(), { 128,0,0,255 }, render, 0);
		opS->Display(390, 170 + 25 * (rdUpdate), render);
		SDL_RenderPresent(render);
		delete opS;
	}
	else if (flag == 1)
	{
		SDL_Delay(1000);
		std::string quote[8] = { "還擊","的防守做的滴水不漏 , 反攻對手","見招拆招 , 反手攻擊","破釜沉舟 , 撲向對方","以四兩力撥千斤擊，重重反打對手","拼命反擊，亂拳揮擊","反咬一口","還手" };
		srand(time(0));
		std::string outputString = "[反擊]: " + name + quote[rand() % 8] + "對 " + other->name + " 造成 " + std::to_string(dmg) + " 點傷害 ";
		Text_UTF8* opS = new Text_UTF8("Fonts/kaiu.ttf", 18, (char*)outputString.c_str(), { 128,0,0,255 }, render, 0);
		opS->Display(390, 170 + 25 * (rdUpdate), render);
		delete opS;
	}
	rdUpdate++;
	if (alive() && flag ==0)
	{
		srand(time(NULL));
		if ((rand() % 100) < stats[3])
			rdUpdate = other->takeDamage(dmg + rand() % (int)(dmg), 1, rdUpdate);
	}
	return rdUpdate;
}

int player::rollSpell(int flag, int round)
{

	SDL_Delay(100);
	srand(time(NULL));
	int slot = rand() % 14;
	if (this->stats[0] == this->max_hp && slot == 0)
	{
		SDL_Delay(123);
		srand(time(NULL));
		slot = rand() % 13 + 1;
	}
	if (flag == 0)
	{
		switch (slot)
		{
		case 0:
		{
			std::string outputString;
			int heal = 80 + rand() % (int)this->stats[5];
			outputString = "[技能]: " + name + " 使用 元氣 回復了" + std::to_string(heal) + "點血量";
			this->stats[0] += heal;
			if (this->stats[0] > this->max_hp) this->stats[0] = this->max_hp;
			Text_UTF8* opS = new Text_UTF8("Fonts/kaiu.ttf", 18, (char*)outputString.c_str(), { 128,0,0,255 }, render, 0);
			opS->Display(390, 170 + 25 * (round), render);
			SDL_Delay(200);
			delete opS;
			break;
		}
		case 1:
		{
			std::string outputString;
			int dmg =23 + rand() % (int)(this->stats[1] / 3 * 5);
			outputString = "[技能]: " + name + " 使用 閃電五連鞭 造成" + std::to_string(dmg) + "點傷害";
			other->takeDamage(dmg, 2, round);
			Text_UTF8* opS = new Text_UTF8("Fonts/kaiu.ttf", 18, (char*)outputString.c_str(), { 128,0,0,255 }, render, 0);
			opS->Display(390, 170 + 25 * (round), render);
			SDL_Delay(200);
			delete opS;
			break;
		}
		case 2:
		{
			std::string outputString;
			int dmg = 61 + rand() % (int)this->stats[5];
			outputString = "[技能]: " + name + " 使用 幻玉鬼火 造成" + std::to_string(dmg) + "點傷害";
			other->takeDamage(dmg, 2, round);
			Text_UTF8* opS = new Text_UTF8("Fonts/kaiu.ttf", 18, (char*)outputString.c_str(), { 128,0,0,255 }, render, 0);
			opS->Display(390, 170 + 25 * (round), render);
			SDL_Delay(200);
			delete opS;
			break;
		}
		case 3:
		{
			std::string outputString;
			outputString = "[技能]: " + name + " 使用詛咒 , " + other->name + " 的能力下降了";
			other->cursed();
			Text_UTF8* opS = new Text_UTF8("Fonts/kaiu.ttf", 18, (char*)outputString.c_str(), { 128,0,0,255 }, render, 0);
			opS->Display(390, 170 + 25 * (round), render);
			SDL_Delay(200);
			delete opS;
			break;
		}
		case 4:
		{
			std::string outputString;
			outputString = "[技能]: " + name + " 服用 禁藥, 以生命力能力作為代價大幅提高能力";
			this->takePill();
			Text_UTF8* opS = new Text_UTF8("Fonts/kaiu.ttf", 18, (char*)outputString.c_str(), { 128,0,0,255 }, render, 0);
			opS->Display(390, 170 + 25 * (round), render);
			SDL_Delay(200);
			delete opS;
			break;
		}
		case 5:
		{
			std::string outputString;
			outputString = "[技能]: " + name + " 對 " + other->name + " 使用 魅惑 , " + other->name + " 放下了戒心 , 防御力下降";
			other->charmed();
			Text_UTF8* opS = new Text_UTF8("Fonts/kaiu.ttf", 18, (char*)outputString.c_str(), { 128,0,0,255 }, render, 0);
			opS->Display(390, 170 + 25 * (round), render);
			SDL_Delay(200);
			delete opS;
			break;
		}
		case 6:
		{
			std::string outputString;
			outputString = "[技能]: " + name + " 使用 金鐘罩 , 防御力提高";
			this->goldenDong();
			Text_UTF8* opS = new Text_UTF8("Fonts/kaiu.ttf", 18, (char*)outputString.c_str(), { 128,0,0,255 }, render, 0);
			opS->Display(390, 170 + 25 * (round), render);
			SDL_Delay(200);
			delete opS;
			break;
		}
		case 7:
		{
			std::string outputString;
			outputString = "[技能]: " + name + " 使用 岩盾 , 捨棄敏捷換來防御力提升";
			this->rockShield();
			Text_UTF8* opS = new Text_UTF8("Fonts/kaiu.ttf", 18, (char*)outputString.c_str(), { 128,0,0,255 }, render, 0);
			opS->Display(390, 170 + 25 * (round), render);
			SDL_Delay(200);
			delete opS;
			break;
		}
		case 8:
		{
			std::string outputString;
			outputString = "[技能]: " + name + " 使用 凌波微步 , 敏捷提升了很多";
			this->sickDodge();
			Text_UTF8* opS = new Text_UTF8("Fonts/kaiu.ttf", 18, (char*)outputString.c_str(), { 128,0,0,255 }, render, 0);
			opS->Display(390, 170 + 25 * (round), render);
			SDL_Delay(200);
			delete opS;
			break;
		}
		case 9:
		{
			std::string outputString;
			outputString = "[技能]: " + name + " 使用 枷鎖 , " + other->name + " 被束縛了 , 敏捷下降";
			other->bondge();
			Text_UTF8* opS = new Text_UTF8("Fonts/kaiu.ttf", 18, (char*)outputString.c_str(), { 128,0,0,255 }, render, 0);
			opS->Display(390, 170 + 25 * (round), render);
			SDL_Delay(200);
			delete opS;
			break;
		}
		case 10:
		{
			std::string outputString;
			outputString = "[道具]: " + name + " 使用 幸運符 , 運氣提高了";
			this->lucky();
			Text_UTF8* opS = new Text_UTF8("Fonts/kaiu.ttf", 18, (char*)outputString.c_str(), { 128,0,0,255 }, render, 0);
			opS->Display(390, 170 + 25 * (round), render);
			SDL_Delay(200);
			delete opS;
			break;
		}
		case 11:
		{
			std::string outputString;
			outputString = "[技能]: " + name + " 向 " + other->name + " 投毒";
			other->takeDamage(other->returnStats(0)*0.4,2,round);
			Text_UTF8* opS = new Text_UTF8("Fonts/kaiu.ttf", 18, (char*)outputString.c_str(), { 128,0,0,255 }, render, 0);
			opS->Display(390, 170 + 25 * (round), render);
			SDL_Delay(200);
			delete opS;
			break;
		}
		case 12:
		{
			std::string outputString;
			int dmg = (int)this->stats[1]*2;
			outputString = "[技能]: " + name + " 不講武德 , 偷偷放暗箭 " + other->name + " 受到了 " + std::to_string(dmg) + " 點傷害";
			other->takeDamage(dmg, 2, round);
			Text_UTF8* opS = new Text_UTF8("Fonts/kaiu.ttf", 18, (char*)outputString.c_str(), { 128,0,0,255 }, render, 0);
			opS->Display(390, 170 + 25 * (round), render);
			SDL_Delay(200);
			delete opS;
			break;
		}
		case 13:
		{
			std::string outputString;
			srand(time(NULL));
			int think = (rand()%1000 == 456);
			if (!think)	outputString = "[技能]: " + name + " 開始冥想 , 但毫無啟悟";
			else
			{
				outputString = "[技能]: " + name + " 開始冥想 , 並悟出真理昇華成神 , 自接制裁了 " + other->name;
				other->smited();
			}
			Text_UTF8* opS = new Text_UTF8("Fonts/kaiu.ttf", 18, (char*)outputString.c_str(), { 128,0,0,255 }, render, 0);
			opS->Display(390, 170 + 25 * (round), render);
			SDL_Delay(200);
			delete opS;
			break;
		}
		case 14:
		{
			break;
		}
		case 15:
		{
			break;
		}
		case 16:
		{
			break;
		}
		}
	}
	round++;
	return round;
}

void player::printOutName(SDL_Renderer* render)
{
	SDL_Color Red = { 220,20,60,255 };
	SDL_Color White = { 255,255,255,255 };
	int font_size = 36;
	int outline_size = 0;
	int gap_size = 73;
	for (int i = 0; i < 6; i++)
	{
		if (this->returnStats(i) > other->returnStats(i))
		{
			Text st1("Fonts/arial.ttf", font_size, std::to_string(int(stats[i])), Red, render, outline_size);
			st1.Display(300, 155 + gap_size * (i), render);

			Text st2("Fonts/arial.ttf", font_size, std::to_string(int(other->returnStats(i))), White, render, outline_size);
			st2.Display(900, 155 + gap_size * (i), render);

		}
		else if (this->returnStats(i) < other->returnStats(i))
		{
			Text st1("Fonts/arial.ttf", font_size, std::to_string(int(stats[i])), White, render, outline_size);
			st1.Display(300, 155 + gap_size * (i), render);

			Text st2("Fonts/arial.ttf", font_size, std::to_string(int(other->returnStats(i))), Red, render, outline_size);
			st2.Display(900, 155 + gap_size * (i), render);
		}
		else
		{
			Text st1("Fonts/arial.ttf", font_size, std::to_string(int(stats[i])), White, render, outline_size);
			st1.Display(300, 155 + gap_size * (i), render);

			Text st2("Fonts/arial.ttf", font_size, std::to_string(int(other->returnStats(i))), White, render, outline_size);
			st2.Display(900, 155 + gap_size * (i), render);
		}
	}
}

int player::normalAttack(int dmg, int round)
{
	int rdUpdate = round;
	int totalDmg = dmg - other->returnStats(2);
	srand(time(NULL));
	if (rand() % 100 > (int)other->returnStats(3))
	{
		if (totalDmg < 20) totalDmg = 16 + rand() % 7;
		rdUpdate = other->takeDamage(totalDmg, 0, round);
	}
	else
	{
		std::string outputString1 = "[攻擊]: " + this->name + " 攻向 " + other->name;
		Text_UTF8* opS1 = new Text_UTF8("Fonts/kaiu.ttf", 18, (char*)outputString1.c_str(), { 128,0,0,255 }, render, 0);
		opS1->Display(390, 170 + 25 * (rdUpdate), render);
		rdUpdate++;
		SDL_RenderPresent(render);
		SDL_Delay(700);
		std::string outputString2 = "[閃避]: " + other->name + " 避開攻擊";
		Text_UTF8* opS2 = new Text_UTF8("Fonts/kaiu.ttf", 18, (char*)outputString2.c_str(), { 128,0,0,255 }, render, 0);
		opS2->Display(390, 170 + 25 * (rdUpdate), render);
		rdUpdate++;

		delete opS1;
		delete opS2;
	}
	if (rdUpdate < 0) return -1;
	return rdUpdate;
}

std::string player::returnName()
{
	return name;
}

void player::cursed()
{
	for (int i = 1; i < 6; i++)
	{
		this->stats[1] *= 0.9;
		floor(this->stats[i]);
	}
}

void player::takePill()
{
	this->stats[0] *= 0.7;
	floor(this->stats[0]);
	for (int i = 1; i < 6; i++)
	{
		this->stats[i] *= 1.2;
		floor(this->stats[i]);
	}
}

void player::charmed()
{
	this->stats[2] *= 0.6;
}

void player::goldenDong()
{
	this->stats[2] += (15);
}

void player::rockShield()
{
	this->stats[3] /= 0.8;
	ceil(this->stats[3]);
	this->stats[2] *= 1.2;
	floor(this->stats[2]);
}

void player::sickDodge()
{
	this->stats[3] += 25;
}

void player::bondge()
{
	this->stats[3] /= 0.5;
	floor(this->stats[3]);
}

void player::lucky()
{
	this->stats[4] += 15;
}

void player::smited()
{
	this->stats[0] = 0;
}




