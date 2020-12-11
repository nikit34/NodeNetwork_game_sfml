#pragma once
#include<vector>

#include <SFML/Network.hpp>

#include "Game.h"
#include "Client.h"



class Server : public Client {
public:
	Server(sf::RenderWindow& win);
	virtual ~Server();

	void launch();

    virtual void actionLink(int orgX, int orgY, int destX, int destY);
    virtual void actionFree(int mouseX, int mouseY);
    virtual void actionFree(sf::Uint8 nidPlayer, int mouseX, int mouseY);
    virtual void actionChangeLink(int orgX, int orgY, int destX, int destY);
    void menuWaitingRoom();
    void waitClient();
    virtual void manageNetwork();
    void addPlayer();

protected:
    void resyncClients();
    sf::TcpListener listener;
    sf::SocketSelector selector;
    std::vector<sf::TcpSocket*> sockets;
    sf::Clock timeElapsed; 
    static const sf::Int32 TIME_RESYNC = 1000; 
};