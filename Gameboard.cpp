#include "Gameboard.h"



Gameboard::Gameboard() { }

//Gameboard::Gameboard(std::vector<Player>* playerList) {
//    this->players = playerList;
//}


void Gameboard::init() {
    randomize();
    initPlayers();
}