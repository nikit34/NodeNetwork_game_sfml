#include "Cell.h"



Cell::Cell(float radius, sf::Vector2f pos, float capacity) :
    radius(radius), pos(pos), capacity(capacity) {

}

Cell::Cell(float radius, float px, float py, float capacity) :
    radius(radius), pos(px, py), capacity(capacity) {

}

Cell::~Cell() { }
