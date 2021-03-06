#pragma once

#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include<iostream>
#include<functional>        
#include<math.h>        
#include<float.h>
#include<vector>
#include<set>
#include<string.h>
#include<string>
#include<sstream>
//#include<unistd.h>

#include "dijkstra.hpp"
#include "player.hpp"
#include "level2.hpp"

#define num 30

using namespace std;
using namespace sf;

class game {

private:

	int grid[num][num];       //map with obstacle
	int posX = 2, posY = 2, destX = 7, destY = 8;
    int gridSize=30;

	int moves = 100;
	int life = 1;
	int hints = 1;
	int rules = 0;
	int bomb = 2;
	int wins = 0;

	Texture background, texPlayer, texPrincess, path, black, emy, imm, texrule, texwin;
	Sprite bg, princess, paths, obstacle, enemy, immunity, ruleset, winpage;
	// audio a;

	Font font;
	Clock clock;
	float dtime = 0.0f;

	dijkstra dj;
	Player player;

	void comments(string image);



public:
	sf::SoundBuffer bgs, attacked, killed, star, hin, win, gover, ride;
	sf::Sound bgsound, attsound, killsound, hinsound, winsound, goversound, starsound, rsound;


	game();
	~game();

	void start(sf::RenderWindow &window);
	void nextlevel(sf::RenderWindow &window);
	void gameOver();
};