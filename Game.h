#pragma once

#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Gameboard.h"



class Game {
public:
	Game();
	void run();
	virtual ~Game();

	void proceed(int orgX, int orgY, int destX, int destY);
	void changeLinkState(int orgX, int orgY, int destX, int destY);
	void manageEvents(sf::Event e);

protected:
	sf::RenderWindow window;
	std::vector<Player> players;

	Gameboard gboard;
	sf::Vector2f posClickedMouse;

	float dt;

	int idPlayer;
};
