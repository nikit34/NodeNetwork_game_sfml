#pragma once

#include <SFML/Network.hpp>


class Client {
public:
    Client(sf::RenderWindow& win);
    virtual ~Client();

    virtual void run();
    void launch();

    void manageEvents(sf::Event event);

protected:
    sf::RenderWindow& window;
};
