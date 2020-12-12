#include "Link.h"



const float Link::TIME_SCALE = 100.f;
const float Link::ARROW_SIZE = 15.f;

Link::Link(Cell* origin, Cell* destination, float strenght) :
    org(origin), dest(destination), strenght(strenght), 
    time(1.f), state(0), arrow(ARROW_SIZE, 3) {
    this->org = origin;
    this->dest = destination;
    this->init();
}

Link::~Link() { }


void Link::init() {
    this->setArrow();
}

inline void Link::setArrow() {
    sf::Vector2f dir = this->org->pos - this->dest->pos;
    dir = dir / (float)sqrt(dir.x * dir.x + dir.y * dir.y);
    this->arrow.setPosition(
        this->dest->pos + (this->dest->radius + this->ARROW_SIZE) * dir
    );
    this->arrow.setFillColor(sf::Color::White);
    this->arrow.setOrigin(this->ARROW_SIZE, this->ARROW_SIZE);
    this->arrow.setRotation((float)atan2(dir.x, dir.y) * -180.f / 3.14f);
}

void Link::update(float dt) {
    this->time -= this->strenght * dt * this->TIME_SCALE;
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
            --this->org->units;
            --this->dest->units;
        }
        this->time = 1.f;
    }
}

void Link::invert() {
    Cell* temp = this->org;
    this->org = this->dest;
    this->dest = temp;
    this->setArrow();
}

void Link::nextState() {
    ++this->state;
    if (this->state == this->COUNT)
        this->state = 0;

    switch (this->state) {
    case 0:
        this->strenght = 0.5f;
        this->arrow.setFillColor(sf::Color(255, 255, 255, 128));
        break;
    case 1:
        this->strenght = 1.f;
        this->arrow.setFillColor(sf::Color(255, 255, 255, 255));
        break; 
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

void Link::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(this->arrow);
}
