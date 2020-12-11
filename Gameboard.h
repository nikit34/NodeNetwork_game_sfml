#pragma once
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "Player.h"
#include "Cell.h"



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
	void update(float dt);
	int getCloserCell(float x, float y);
	int getCloserCell(sf::Vector2f coord);

protected:
	std::vector<Player>* players;
	std::vector<Cell> cells;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};