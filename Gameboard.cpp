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
    
}


void Gameboard::update() {

}

