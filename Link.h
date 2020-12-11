#pragma once

#include <SFML/Graphics.hpp>

#include "Cell.h"



class Link : public sf::Drawable {
public:
	Link(Cell* origin, Cell* destination, float strenght = 1.f);
    virtual ~Link();
    sf::Vector2f getOrgPos() const;
    sf::Vector2f getDestPos() const;
    float getDestRadius() const;
    void init();

    void update(float dt);

protected:
    Cell* org;
    Cell* dest;
    float strenght;
    float time;

    static const float TIME_SCALE;

private:
    friend class Gameboard;
};
