#include "ClientMenuServerList.h"



MenuServerList::MenuServerList(sf::RenderWindow& win, sf::UdpSocket& sock_udp, bool& n_connected, sf::IpAddress& ip_serv, sf::Uint8& servStatus) :
    Menu(win), socket_udp(sock_udp), connected(n_connected),
    serverAddress(ip_serv), serverStatus(servStatus) {
    sf::Font font;
    font.loadFromFile("Textures/JetBreins.ttf");

    this->textManualIp.setFont(font);
    this->textManualIp.setCharacterSize(24);
    this->textManualIp.setString("Manual ip  : ");
    this->textManualIp.setPosition(10.f, FIRST_LINE_Y);

    this->buttonManualIp.setFont(font);
    this->buttonManualIp.setCharacterSize(24);
    this->buttonManualIp.setString("Try connection");
    this->buttonManualIp.setPosition(
        this->window.getSize().x - 
        this->buttonManualIp.getGlobalBounds().width - 
        20.f, 
        this->FIRST_LINE_Y
    );

    this->POSBAR_X = this->textManualIp.getGlobalBounds().width
        + this->textManualIp.getGlobalBounds().left + 20.f;

    this->infoInput.setFont(font);
    this->infoInput.setCharacterSize(24);
    this->infoInput.setString("");
    this->infoInput.setPosition(this->POSBAR_X, this->POSBAR_Y);
    std::string textInput("");
    sf::IpAddress directIp;

    this->textBar.setFont(font);
    this->textBar.setCharacterSize(24);
    this->textBar.setString("|");
    this->textBar.setPosition(this->POSBAR_X, this->POSBAR_Y);

    const float SECOND_LINE_Y = this->FIRST_LINE_Y + 30.f;
    this->info.setFont(font);
    this->info.setCharacterSize(24);
    this->info.setString("Ip adresses ---------------------------");
    this->info.setPosition(10.f, SECOND_LINE_Y);

    this->info2.setFont(font);
    this->info2.setCharacterSize(18);
    this->info2.setString("<empty>");
    this->info2.setPosition(10.f, SECOND_LINE_Y);
    this->info2.move(0, SECOND_LINE_Y);


    this->window.setFramerateLimit(60);
}

MenuServerList::~MenuServerList() { }


void MenuServerList::run()
{
}
