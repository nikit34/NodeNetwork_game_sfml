#include "Link.h"


const float Link::TIME_SCALE = 100.f;

Link::Link(Cell* origin, Cell* destination, float strenght) :
    org(origin), dest(destination), strenght(strenght), time(1.f) {
    
    this->org = origin;
    this->dest = destination;
    this->init();
}

Link::~Link() { }


void Link::init() {
    
}

void Link::update(float dt) {
    this->time -= this->strenght * dt * TIME_SCALE;
    if (this->time > 0.f) 
        return;
    if (this->org->units > 0) {
        if (this->dest->units == 0) {
            --this->org->units;
            ++this->dest->units;
            this->dest->idOwner = this->org->idOwner;
        }
        else if (this->dest->idOwner == this->org->idOwner) {
            if (this->dest->units < this->dest->capacity) {
                --this->org->units;
                ++this->dest->units;
            }
        }
        else {
            this->org->units--;
            this->dest->units--;
        }
        this->time = 1.f;
    }
}

sf::Vector2f Link::getOrgPos() const {
    return this->org->pos;
}
sf::Vector2f Link::getDestPos() const {
    return this->dest->pos;
}

float Link::getDestRadius() const {
    return this->dest->radius;
}
