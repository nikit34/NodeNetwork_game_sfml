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
    this->connected = false;
    this->exitCurrentGame = false;

    this->gboard.init();
    this->serverAddress = sf::IpAddress::None;
    sf::Thread thread(&Client::waitServer, &(*this));

    thread.launch();

    // open UDP
    this->socket_udp.setBlocking(false);
    if (this->socket_udp.bind(this->PORT_GAME_B) != sf::Socket::Done)
        std::cout << "[ERROR] UDP error open" << std::endl;


    this->menuServerList();

    if (!this->window.isOpen()) {
        this->start = true;
        thread.terminate();
        return;
    }

    this->socket_udp.unbind();

    this->menuWaitingRoom();

    thread.terminate();

    if (!this->exitCurrentGame) {
        this->socket.setBlocking(true);
        sf::Packet packet;
        this->socket.receive(packet);
        if (packet >> this->gboard) {
            std::cout << "Carte recue" << std::endl;
            this->run();
        }
        else
            std::cout << "erreur de reception du jeu ..." << std::endl;
    }
}

void Client::run() {
    sf::Clock clock;
    this->socket.setBlocking(false);
    float dt;
    sf::Event event;
    while (this->window.isOpen() && !this->exitCurrentGame && this->connected) {
        dt = clock.restart().asSeconds();
 
        while (this->window.pollEvent(event))
            manageEvents(event);
        
        manageNetwork();

        this->gboard.update(dt);
        this->window.clear();
        this->window.draw(this->gboard);
        this->window.display();
    }
    this->socket.disconnect();
}

void Client::manageEvents(sf::Event event) {
    if (event.type == sf::Event::Closed)
        this->window.close();

    if (event.type == sf::Event::KeyPressed)
        if (event.key.code == sf::Keyboard::Escape) 
            this->exitCurrentGame = true;
        
    
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) 
            this->posClickedMouse = this->window.mapPixelToCoords(sf::Mouse::getPosition(this->window));
        
        else if (event.mouseButton.button == sf::Mouse::Right) 
            this->posClickedMouse = this->window.mapPixelToCoords(sf::Mouse::getPosition(this->window));
        
    }

    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            this->actionLink(this->posClickedMouse.x, this->posClickedMouse.y,
                this->window.mapPixelToCoords(sf::Mouse::getPosition(this->window)).x,
                this->window.mapPixelToCoords(sf::Mouse::getPosition(this->window)).y
            );
        }
        else if (event.mouseButton.button == sf::Mouse::Right) {
            if (this->gboard.getCloserCell(
                this->posClickedMouse.x, this->posClickedMouse.y
            ) == this->gboard.getCloserCell(
                this->window.mapPixelToCoords(sf::Mouse::getPosition(this->window))
                )
            ) {
                this->actionFree(this->idPlayer, this->posClickedMouse.x, this->posClickedMouse.y);
            }
            else {
                this->actionChangeLink(this->posClickedMouse.x, this->posClickedMouse.y,
                    this->window.mapPixelToCoords(sf::Mouse::getPosition(this->window)).x,
                    this->window.mapPixelToCoords(sf::Mouse::getPosition(this->window)).y);
            }
        }
    }
}

void Client::actionLink(int orgX, int orgY, int destX, int destY) {
    int idOrg = this->gboard.getCloserCell(orgX, orgY);
    if (idOrg >= 0 && this->gboard.getOwner(idOrg) == this->idPlayer) {
        int idDest = this->gboard.getCloserCell(destX, destY);
        this->gboard.link(idOrg, idDest);
        sf::Packet packet;
        sf::Uint8 code = this->LINK;
        sf::Uint16 org = idOrg;
        sf::Uint16 dest = idDest;
        packet << code << org << dest;
        this->socket.send(packet);
    }
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

void Client::manageNetwork() {
    if (!this->connected)
        return;
    sf::Packet packet;
    if (this->socket.receive(packet) == sf::Socket::Disconnected) {
        std::cout << "[ERROR] connected" << std::endl;
        this->connected = false;
    }
    sf::Uint8 code;
    if (packet >> code) 
        networkActions(packet, code);
}

void Client::networkActions(sf::Packet& p, sf::Uint8 code)
{
}

void Client::waitServer() {
    while (this->serverAddress == sf::IpAddress::None) 
        sf::sleep(sf::milliseconds(1000));
    
    std::cout << "Try connect: " + this->serverAddress.toString() << std::endl;

    sf::Socket::Status status = socket.connect(this->serverAddress, this->PORT_GAME_A);
    if (status != sf::Socket::Done) {
        std::cout << "Impossible connecting..." << std::endl;
        this->serverAddress = sf::IpAddress::None;
        this->waitServer();
        return;
    }
    else {
        std::cout << "connected!" << std::endl;
        this->connected = true;
    }

    std::cout << "waiting for other players..." << std::endl;

    this->socket.setBlocking(false);
    sf::Packet packetInfo;
    this->start = false;
    while (!this->start && !this->exitCurrentGame) {
        if (this->socket.receive(packetInfo) == sf::Socket::Done) {
            if (packetInfo >> this->playerConnected >> this->playerMax >> this->serverStatus) {
                if (this->idPlayer == 0) {
                    this->idPlayer = this->playerConnected;
                    std::cout << "Your ID: " << (int)this->idPlayer << std::endl;
                }
            }
        }
        if(this->playerConnected == this->playerMax){
            std::cout << "All ready!" << std::endl;
        }
        if (this->serverStatus == this->SERVER_START_GAME) {
            this->start = true;
            break;
        }
    }
    if (this->start && !this->exitCurrentGame) {
        for (int i = 1; i < (int)this->playerMax; ++i) {
            this->players.push_back(Player(1 + i));
        }
    }
}

void Client::menuWaitingRoom()
{
}

void Client::menuServerList()
{
}
