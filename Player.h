#pragma once

#include <SFML/Graphics.hpp>



class Player {
public:
	Player(int id, sf::Color color);
	virtual ~Player();

	sf::Color getColor() const { return this->color; }
	int getId() const { return this->id; }

protected:
	int id;
	sf::Color color;
};
