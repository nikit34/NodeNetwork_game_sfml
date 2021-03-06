#pragma once
#include <iostream>
#include <vector>

#include <SFML/Network.hpp>

#include "Game.h"
#include "ClientMenuServerList.h"



class Client {
public:
    Client(sf::RenderWindow& win);
    virtual ~Client();

    virtual void run();
    void launch();

    void manageEvents(sf::Event event);

    virtual void actionLink(int orgX, int orgY, int destX, int destY);
    virtual void actionFree(int mouseX, int mouseY);
    virtual void actionFree(sf::Uint8 nidPlayer, int mouseX, int mouseY);
    virtual void actionChangeLink(int orgX, int orgY, int destX, int destY);
    virtual void manageNetwork();
    void networkActions(sf::Packet& p, sf::Uint8 code);
    void waitServer();
    enum CODE { LINK, FREE, CHANGE, SYNCHRONIZE_GBOARD };
    enum STATE { SERVER_READY, SERVER_NOT_READY, SERVER_START_GAME };
    virtual void menuWaitingRoom();
    void menuServerList();

protected:
    sf::RenderWindow& window;
    Gameboard gboard;
    std::vector<Player> players;
    sf::Uint8 idPlayer;

    sf::Vector2f posClickedMouse;

    sf::Uint8 serverStatus;
    sf::Uint8 playerConnected;
    sf::Uint8 playerMax;

    sf::TcpSocket socket;
    sf::UdpSocket socket_udp;
    sf::IpAddress serverAddress;

    static const unsigned short int PORT_GAME_A = 53000; // tcp
    static const unsigned short int PORT_GAME_B = 54000; // udp

    bool start;
    bool connected;
    bool exitCurrentGame;
};
