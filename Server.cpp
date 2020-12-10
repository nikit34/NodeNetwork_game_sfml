#include "Server.h"



Server::Server(sf::RenderWindow& win) : Client(win) {
	// !!! window is client !!!
	this->window.setTitle("server");

}

Server::~Server() { }


void Server::launch() {

}