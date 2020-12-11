#include "Client.h"



Client::Client(sf::RenderWindow& win) : 
    window(win), gboard(&players), idPlayer(0),
    start(false), connected(false), exitCurrentGame(false) {
	this->window.setTitle("client");
}

Client::~Client() { }


void Client::launch() {
    start = false;
    connected = false;
    exitCurrentGame = false;

    gboard.init();
}

void Client::run() {

}

void Client::manageEvents(sf::Event event) {

}