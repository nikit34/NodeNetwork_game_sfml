#include "Client.h"



Client::Client(sf::RenderWindow& win) : 
    window(win), gboard(&players), exitCurrentGame(false), 
    posClickedMouse(-1, -1), idPlayer(0),
    connected(false), start(false), playerConnected(1) {
	this->window.setTitle("client");
    this->idPlayer = 0;
    this->playerMax = 1;
    this->players.push_back(Player(1));
}

Client::~Client() { }


void Client::launch() {
    this->start = false;
    connected = false;
    exitCurrentGame = false;

    gboard.init();
}

void Client::run() {

}

void Client::manageEvents(sf::Event event) {

}