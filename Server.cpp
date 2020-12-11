#include "Server.h"



Server::Server(sf::RenderWindow& win) : Client(win) {
	// !!! window is client !!!
	this->window.setTitle("server");
	this->idPlayer = 1;
}

Server::~Server() { }


void Server::launch() {
	this->start = false;
	this->connected = false;
	this->exitCurrentGame = false;

	this->gboard.init();
}

void Server::manageEvents(sf::Event event)
{
}

void Server::actionLink(int orgX, int orgY, int destX, int destY)
{
}

void Server::actionFree(int mouseX, int mouseY)
{
}

void Server::actionFree(sf::Uint8 nidPlayer, int mouseX, int mouseY)
{
}

void Server::actionChangeLink(int orgX, int orgY, int destX, int destY)
{
}

void Server::menuWaitingRoom()
{
}

void Server::waitClient()
{
}

void Server::waitAll()
{
}

void Server::manageNetwork()
{
}

void Server::addPlayer()
{
}

void Server::resyncClients()
{
}
