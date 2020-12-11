#pragma once
#include <vector>
#include <deque>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "Player.h"
#include "Cell.h"
#include "Link.h"



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

	friend sf::Packet& operator>>(sf::Packet& packet, Gameboard& gameboard);
	friend sf::Packet& operator<<(sf::Packet& packet, const Gameboard& gameboard);

protected:
	std::vector<Player>* players;
	std::vector<Cell> cells;
	std::deque<Link> links;
	static sf::Clock elapsed;
	std::vector<float> frequenceRotation;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	static const uint16_t NB;
	static const float MIN_DIST;
	static const float BORDER;
	static const uint16_t MIN_CAPACITY;
	static const uint16_t MAX_CAPACITY;
	static const float WIDTH;
	static const float HEIGHT;
};


sf::Packet& operator>>(sf::Packet& packet, Gameboard& gboard);
sf::Packet& operator<<(sf::Packet& packet, const Gameboard& gboard);