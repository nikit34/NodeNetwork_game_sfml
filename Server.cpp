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

	sf::Thread thread(&Server::waitClient, &(*this));
	thread.launch();

	this->socket_udp.setBlocking(false);
	this->menuWaitingRoom();

	thread.terminate();

	if (!this->window.isOpen())
		return;
	
	if (this->connected) {
		this->serverStatus = SERVER_START_GAME;
		
		sf::Packet packet;

		for (std::vector<sf::TcpSocket*>::iterator it = this->sockets.begin(); it != this->sockets.end(); ++it) {
			sf::TcpSocket& client = **it;
			
			packet << this->playerConnected << this->playerMax << this->serverStatus;

			if (client.send(packet) == sf::Socket::Done)
				std::cout << "Send push sequence to " << client.getRemoteAddress().toString() << std::endl;
			
		}

		packet << this->gboard;

		uint64_t id = 0;
		for (std::vector<sf::TcpSocket*>::iterator it = this->sockets.begin(); it != this->sockets.end(); ++it) {
			sf::TcpSocket& client = **it;
			if (client.send(packet) == sf::Socket::Done)
				std::cout << "Carte be send " << int(id + 2) << std::endl;
			client.setBlocking(false);
			++id;
		}

		this->timeElapsed.restart();
		this->run();
	}
	else {
		std::cout << "Solo game" << std::endl;
		this->timeElapsed.restart();
		this->run();
	}
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
