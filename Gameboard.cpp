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

void Gameboard::draw(sf::RenderTarget& target, sf::RenderStates states) const {

}

void Gameboard::link(int idOrg, int idDest, float strength) {
    
}

void Gameboard::update(float dt) {
    for (std::vector<Cell>::iterator it = cells.begin(); it != cells.end(); ++it) {
        it->update(dt);
    }
}

int Gameboard::getCloserCell(float x, float y) {
    int id = 0;
    float dx, dy, sr;
    for (std::vector<Cell>::iterator it = cells.begin(); it != cells.end(); it++) {
        dx = it->pos.x - x;
        dy = it->pos.y - y;
        sr = it->radius;
        if (dx * dx + dy * dy < sr * sr)
            return id;
        ++id;
    }
    return -1;
}

int Gameboard::getCloserCell(sf::Vector2f coord) {
    return getCloserCell(coord.x, coord.y);
}

void Gameboard::free(int idPlayer, int idCell) {

}

void Gameboard::changeLinkState(int idOrg, int idDest) {

}

int Gameboard::getOwner(int idCell) {
    return this->cells.at(idCell).idOwner;
}
