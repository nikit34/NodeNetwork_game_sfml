#pragma once

#include <SFML/Network.hpp>



class Server : public Client {
public:
	Server(sf::RenderWindow& win);
	virtual ~Server();

	virtual void run();
	void launch();


};