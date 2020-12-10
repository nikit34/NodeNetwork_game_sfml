#pragma once

#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Gameboard.h"



class Game {
public:
	Game();
	void run();
	virtual ~Game();

protected:
	sf::RenderWindow window;
	std::vector<Player> players;

	Gameboard gboard;
};
