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

void Client::actionLink(int orgX, int orgY, int destX, int destY)
{
}

void Client::actionFree(int mouseX, int mouseY)
{
}

void Client::actionFree(sf::Uint8 nidPlayer, int mouseX, int mouseY)
{
}

void Client::actionChangeLink(int orgX, int orgY, int destX, int destY)
{
}

void Client::manageNetwork()
{
}

void Client::networkActions(sf::Packet& p, sf::Uint8 code)
{
}

void Client::waitServer()
{
}

void Client::menuWaitingRoom()
{
}

void Client::menuServerList()
{
}
