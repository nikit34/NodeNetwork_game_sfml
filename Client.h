#pragma once
#include <iostream>
#include <vector>

#include <SFML/Network.hpp>

#include "Game.h"
#include "Server.h"



class Client {
public:
    Client(sf::RenderWindow& win);
    virtual ~Client();

    virtual void run();
    void launch();

    void manageEvents(sf::Event event);

protected:
    sf::RenderWindow& window;
    Gameboard gboard;
    std::vector<Player> players;
    sf::Uint8 idPlayer;

    sf::Vector2f posClickedMouse;

    sf::Uint8 serverStatus;
    sf::Uint8 playerConnected;
    sf::Uint8 playerMax;

    bool start;
    bool connected;
    bool exitCurrentGame;
};
