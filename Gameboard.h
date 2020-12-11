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
	void link(int idOrg, int idDest, float strength=0.5f);
	void changeLinkState(int idOrg, int idDest);
	int getCloserCell(float x, float y);
	int getCloserCell(sf::Vector2f coord);
	int getOwner(int idCell);
	void free(int idPlayer, int idCell);

protected:
	std::vector<Player>* players;
	std::vector<Cell> cells;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};