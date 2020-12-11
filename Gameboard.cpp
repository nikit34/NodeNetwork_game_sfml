#include "Gameboard.h"



Gameboard::Gameboard() { }

Gameboard::Gameboard(std::vector<Player>* playerList) {
    this->players = playerList;
}

Gameboard::~Gameboard() { }


void Gameboard::init() {
    this->randomize();
    this->initPlayers();
}


void Gameboard::reset() {
    this->cells.clear();
}


void Gameboard::randomize() {

}


void Gameboard::initPlayers() {
    for (uint64_t i = 0; i < this->cells.size(); ++i) {
        this->cells[i].idOwner = 0;
    }
    int c;
    for (uint64_t i = 1; i < (*this->players).size() + 1; ++i) {
        c = rand() % this->cells.size();
        if (this->cells[c].idOwner == 0) {
            this->cells[c].idOwner = i;
            this->cells[c].capacity += 100;
            this->cells[c].units += 100;
        }
        else
            --i;
    }
}

void Gameboard::update(float dt) {
    for (std::vector<Cell>::iterator it = cells.begin(); it != cells.end(); ++it) {
        it->update(dt);
    }
}

int Gameboard::getCloserCell(sf::Vector2f coord) {
    return getCloserCell(coord.x, coord.y);
}

void Gameboard::draw(sf::RenderTarget& target, sf::RenderStates states) const {

}

