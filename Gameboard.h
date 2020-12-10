#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "Player.h"



// Drawable - for paint
class Gameboard : public sf::Drawable {
public:
	Gameboard();
	Gameboard(std::vector<Player>* playerList);
	virtual ~Gameboard();

	void init();
	void reset();
	void randomize();
	void initPlayers();
	void update();

protected:
	std::vector<Player>* players;
	//std::vector<Cell> cells;
};