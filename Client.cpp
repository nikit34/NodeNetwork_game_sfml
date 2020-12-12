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
            std::cout << "Card received" << std::endl;
            this->run();
        }
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

void Client::actionFree(int mouseX, int mouseY) {
    sf::Uint16 idCell = this->gboard.getCloserCell(mouseX, mouseY);
    gboard.free(this->idPlayer, idCell);
    sf::Packet packet;
    sf::Uint8 code = this->FREE;
    sf::Uint16 org = idCell;
    packet << code << this->idPlayer << org;
    this->socket.send(packet);
}

void Client::actionFree(sf::Uint8 nidPlayer, int mouseX, int mouseY) {
    sf::Uint16 idCell = this->gboard.getCloserCell(mouseX, mouseY);
    this->gboard.free(nidPlayer, idCell);
    sf::Packet packet;
    sf::Uint8 code = this->FREE;
    sf::Uint16 org = idCell;
    packet << code << nidPlayer << org;
    this->socket.send(packet);
}


void Client::actionChangeLink(int orgX, int orgY, int destX, int destY) {
    int idOrg = this->gboard.getCloserCell(orgX, orgY);
    if (idOrg >= 0 && this->gboard.getOwner(idOrg) == this->idPlayer) {
        int idDest = this->gboard.getCloserCell(destX, destY);
        if (idDest >= 0) {
            this->gboard.changeLinkState(idOrg, idDest);
            sf::Packet packet;
            sf::Uint8 code = this->CHANGE;
            sf::Uint16 org = idOrg;
            sf::Uint16 dest = idDest;
            packet << code << org << dest;
            this->socket.send(packet);
        }
    }
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

void Client::networkActions(sf::Packet& packet, sf::Uint8 code) {
    sf::Uint16 org, dest;
    sf::Uint8 nidPlayer;
    if (code == this->LINK) {
        packet >> org >> dest;
        if (org >= 0 && dest >= 0)
            this->gboard.link(org, dest);
    }
    else if (code == this->CHANGE) {
        packet >> org >> dest;
        if (org >= 0 && dest >= 0)
            this->gboard.changeLinkState(org, dest);
    }
    else if (code == this->FREE) {
        packet >> nidPlayer >> org;
        this->gboard.free(nidPlayer, org);
    }
    else if (code == this->SYNCHRONIZE_GBOARD) {
        packet >> this->gboard;
    }
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

void Client::menuWaitingRoom() {
    sf::Text info;
    info.setFont(*FontManager::getFont("Textures/JetBreins.ttf"));
    info.setCharacterSize(24);
    info.setPosition(10.f, 10.f);

    sf::Text info2;
    info2.setFont(*FontManager::getFont("Textures/JetBreins.ttf"));
    info2.setCharacterSize(18);
    info2.setPosition(10.f, 10.f);
    info2.move(0, 40.f);

    sf::Text info3 = info;
    info3.setPosition(300.f, 200.f);
    info3.setString("Player color :");

    sf::RectangleShape colorRect(sf::Vector2f(50.f, 20.f));
    colorRect.setPosition(330.f + info3.getGlobalBounds().width, 200.f);
    colorRect.setFillColor(Player::DEFAULT_COLOR[this->idPlayer]);

    info.setString("waiting ...");
    sf::Event event;
    while (!this->start && !this->exitCurrentGame && this->window.isOpen()) {
        while (this->window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                this->exitCurrentGame = true;
                this->window.close();
            }
            if (event.type == sf::Event::KeyPressed) 
                if (event.key.code == sf::Keyboard::Escape) 
                    this->exitCurrentGame = true;
        }

        info2.setString("Connectes: " + std::to_string((int)this->playerConnected) + " / " + std::to_string((int)this->playerMax));

        this->window.clear();
        this->window.draw(info);
        this->window.draw(info2);
        this->window.draw(info3);
        this->window.draw(colorRect);

        this->window.display();
    }
}

void Client::menuServerList() {
    MenuServerList menu(
        this->window, 
        this->socket_udp, 
        this->connected, 
        this->serverAddress, 
        this->serverStatus
    );
    menu.run();
    return;
}
