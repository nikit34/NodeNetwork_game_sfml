#pragma once

#include <SFML/Graphics.hpp>



class Player {
public:
	Player(int id);
	Player(int id, sf::Color color);
	virtual ~Player();

	static sf::Color DEFAULT_COLOR[];

	sf::Color getColor() const { return this->color; }
	int getId() const { return this->id; }

protected:
	int id;
	sf::Color color;
};
