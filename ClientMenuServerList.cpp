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


template<typename T>
std::string ttos(T t) {
    std::stringstream ss;
    std::string s;
    if (ss << t) {
        ss >> s;
        return s;
    }
    return "";
}

void MenuServerList::run() {
    bool exitCurrentGame = false;
    std::string textInput("");
    while (!this->connected && !exitCurrentGame && this->window.isOpen()) {
        if (this->clock.getElapsedTime().asMilliseconds() > 1000) {
            this->clock.restart();
        }
        sf::Event event;
        while (this->window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                this->window.close();
                this->connected = true;
            }
            else
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        exitCurrentGame = true;
                        this->connected = true;
                    }
                }
                else
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            for (unsigned int i = 0; i < this->listServButton.size(); ++i) {
                                if (this->listServButton[i].getGlobalBounds().contains(
                                    this->window.mapPixelToCoords(
                                        sf::Mouse::getPosition(this->window)
                                    )
                                )) {
                                    this->serverAddress = sf::IpAddress(this->listServIp[i]);
                                }
                            }

                            if (this->buttonManualIp.getGlobalBounds().contains(
                                this->window.mapPixelToCoords(
                                    (sf::Mouse::getPosition(this->window))))
                                && this->directIp != sf::IpAddress::None) {
                                this->serverAddress = this->directIp;
                            }
                        }
                    }
                    else
                        if (event.type == sf::Event::TextEntered) {
                            if (event.text.unicode < 128) {
                                if (event.text.unicode == '\b') {
                                    if (textInput.size() > 0) {
                                        textInput = textInput.substr(0, textInput.size() - 1);
                                    }
                                }
                                else
                                    if (textInput.size() < 15) {
                                        if (((event.text.unicode >= '0' &&
                                            event.text.unicode <= '9')))
                                        {
                                            textInput += event.text.unicode;
                                            if (((textInput.size() + 1) % 4 == 0)
                                                && textInput.size() > 0
                                                && textInput.size() < 11) {
                                                textInput.push_back('.');
                                            }
                                        }
                                        else {
                                            if (event.text.unicode == '.'
                                                && (textInput.size() + 1) % 4 > 0)
                                            {
                                                uint64_t a = textInput.size() % 4;
                                                std::string tmp = textInput.substr(textInput.size() - a, a);
                                                textInput = textInput.substr(0, textInput.size() - a);
                                                for (uint64_t i = 0; i < 3 - a; ++i) {
                                                    textInput.push_back('0');
                                                }
                                                textInput += tmp;
                                                if (textInput.size() <= 11)
                                                    textInput.push_back('.');
                                            }
                                        }
                                    }

                                this->infoInput.setString(textInput);
                                if (textInput.size() >= 12)
                                    this->directIp = sf::IpAddress(textInput);
                                this->textBar.setPosition(this->POSBAR_X + this->infoInput.getGlobalBounds().width, this->POSBAR_Y);
                            }
                        }
        }

        if (this->socket_udp.receive(this->info_server, this->serv_ip, this->serv_port) != sf::Socket::Done) 
            std::cout << "[ERROR] receive UDP " << std::endl;
        else {
            if (this->info_server >> this->serverStatus) {
                std::string s = "serv: " + ttos(this->serv_ip);
                s += " - ";
                if (this->serverStatus == Client::SERVER_READY) {
                    s += "ready !";
                }
                else {
                    s += "not ready";
                }
                s += "\n";
                bool founded = false;
                for (unsigned int i = 0; i < this->listServIp.size(); ++i) {
                    if (this->listServIp[i] == this->serv_ip.toInteger()) {
                        founded = true;
                        this->listServText[i] = s;
                        this->listServButton[i].setString(this->listServText[i]);
                        this->listServTimeout[i].restart();
                        break;
                    }
                }
                // correct the lists if not founded
                if (!founded) {
                    this->listServIp.push_back(this->serv_ip.toInteger());
                    this->listServText.push_back(s);
                    this->listServButton.push_back(this->info2);
                    this->listServButton.back().setString(this->listServText.back());
                    this->listServTimeout.push_back(sf::Clock());
                }

            }
        }
        for (uint64_t i = 0; i < this->listServIp.size(); ++i) {
            if (this->listServTimeout[i].getElapsedTime().asSeconds() > 1.f) {
                this->serv_ip = sf::IpAddress(this->listServIp[i]);
                std::string s = "serv: " + this->serv_ip.toString() + " - < Disconnected >";
                this->listServText[i] = s;
                this->listServButton[i].setString(this->listServText[i]);
            }
            if (this->listServButton[i].getGlobalBounds().contains(
                this->window.mapPixelToCoords(
                    sf::Mouse::getPosition(this->window)))) {
                this->listServButton[i].setFillColor(sf::Color(255, 255, 200, 128));
            }
            else {
                this->listServButton[i].setFillColor(sf::Color(255, 255, 255, 255));
            }
        }
        if (this->buttonManualIp.getGlobalBounds().contains(this->window.mapPixelToCoords(
            (sf::Mouse::getPosition(this->window))))
            && this->directIp != sf::IpAddress::None)
        {
            this->buttonManualIp.setFillColor(sf::Color(200, 200, 255, 200));
        } 
        else {
            this->buttonManualIp.setFillColor(sf::Color(255, 255, 255, 255));
        }

         this->window.clear();
         this->window.draw(this->info);
         this->window.draw(this->textManualIp);
         this->window.draw(this->buttonManualIp);
         this->window.draw(this->infoInput);

        if (this->clock.getElapsedTime().asMilliseconds() > 500) {
            this->window.draw(this->textBar);
        }
        if (this->listServIp.size() <= 0) {
            this->window.draw(this->info2);
        }
        else {
            for (uint64_t i = 0; i < this->listServIp.size(); ++i) {
                this->window.draw(this->listServButton[i]);
            }
        }
        this->window.display();
    }
}

