#pragma once

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>



class Cell {
public:
	Cell(float radius, sf::Vector2f pos, float capacity);
	Cell(float radius, float posx, float posy, float capacity);
	virtual ~Cell();

protected:
	float radius;
	sf::Vector2f pos;
	sf::Uint16 capacity;
};