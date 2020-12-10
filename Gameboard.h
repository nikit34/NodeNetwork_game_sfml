#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>


class Gameboard : public sf::Drawable {
public:
	Gameboard();
	//Gameboard(std::vector<Player> playerList);
	virtual ~Gameboard();

	void init();
	void reset();
	void randomize();
	void initPlayers();
	void update();

protected:
	//std::vector<Player>* players;
};