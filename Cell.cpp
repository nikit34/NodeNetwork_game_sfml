#include "Cell.h"



Cell::Cell(float radius, sf::Vector2f pos, float capacity) :
    radius(radius), pos(pos), capacity(capacity),
    idOwner(0), units(0), time(1.f) {

}

Cell::Cell(float radius, float px, float py, float capacity) :
    radius(radius), pos(px, py), capacity(capacity),
    idOwner(0), units(0), time(1.f) {

}

Cell::~Cell() { }


const float Cell::TIME_SCALE = 0.1f;

void Cell::update(float dt) {
    this->time -= dt * this->radius * this->TIME_SCALE;
    if (this->time < 0.f) {
        ++this->units;
        if (this->units > this->capacity)
            this->units = this->capacity;
        this->time = 1.f;
    }
}

void Cell::setOwner(int ownerId) {
    this->idOwner = ownerId;
}

int Cell::getOwner() const {
    return this->idOwner;
}