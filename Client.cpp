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

}

void Client::manageEvents(sf::Event event) {

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

void Client::manageNetwork()
{
}

void Client::networkActions(sf::Packet& p, sf::Uint8 code)
{
}

void Client::waitServer()
{
}

void Client::menuWaitingRoom()
{
}

void Client::menuServerList()
{
}
