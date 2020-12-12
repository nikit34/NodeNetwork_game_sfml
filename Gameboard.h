#pragma once
#include <vector>
#include <deque>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "Player.h"
#include "Link.h"
#include "Manager.h"



#define RAD_TO_DEG 57.2957795f

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

	inline sf::Vector2f dot(sf::Vector2f a, sf::Vector2f b) const;

protected:
	std::vector<Player>* players;
	std::vector<Cell> cells;
	std::deque<Link> links;
	static sf::Clock elapsed;
	std::vector<float> frequenceRotation;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	static const uint16_t NUMBER;
	static const float MIN_DIST;
	static const float BORDER;
	static const uint16_t MIN_CAPACITY;
	static const uint16_t MAX_CAPACITY;
	static const float WIDTH;
	static const float HEIGHT;
};


sf::Packet& operator>>(sf::Packet& packet, Gameboard& gboard);
sf::Packet& operator<<(sf::Packet& packet, const Gameboard& gboard);


inline sf::Vector2f Gameboard::dot(sf::Vector2f a, sf::Vector2f b) const {
	return sf::Vector2f(a.x * b.x, a.y * b.y);
}

template<typename T>
std::string ttos(T t){
	std::stringstream ss;
	std::string s;
	if (ss << t) {
		ss >> s;
		return s;
	}
	return "";
}