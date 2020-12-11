#pragma once

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

class Gameboard;


class Cell {
public:
    Cell() {};
	Cell(float radius, sf::Vector2f pos, float capacity);
	Cell(float radius, float posx, float posy, float capacity);
    virtual ~Cell();

    void setOwner(int ownerId);
    int getOwner() const;
    void update(float dt);

protected:
    float radius;
    sf::Vector2f pos;
    sf::Uint16 capacity;

    sf::Uint8 idOwner;
    sf::Uint16 units;

    float time;
    static const float TIME_SCALE;

private:
	friend class Gameboard;
	friend class Link;

    friend sf::Packet& operator>> (sf::Packet& packet, Gameboard& gboard);
    friend sf::Packet& operator<< (sf::Packet& packet, const Gameboard& gboard);
};

class Gameboard;
sf::Packet& operator>> (sf::Packet& packet, Gameboard& gboard);
sf::Packet& operator<< (sf::Packet& packet, const Gameboard& gboard);