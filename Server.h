#pragma once

#include <SFML/Network.hpp>

#include "Game.h"
#include "Client.h"



class Server : public Client {
public:
	Server(sf::RenderWindow& win);
	virtual ~Server();

	virtual void run();
	void launch();


};