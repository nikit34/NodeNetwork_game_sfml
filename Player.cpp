#include "Player.h"



Player::Player(int id) : id(id) {
    if (this->id > 9)
        this->color = sf::Color(250, 250, 250);
    else
        this->color = DEFAULT_COLOR[Id];
}

Player::Player(int id, sf::Color color) : id(id), color(color) { }

Player::~Player() { }


sf::Color Player::DEFAULT_COLOR[] =
{ sf::Color(128, 128, 128),
sf::Color(0, 255, 0),
sf::Color(255, 0, 0),
sf::Color(0, 0, 255),
sf::Color(255, 0, 255),
sf::Color(255, 255, 0),
sf::Color(0, 255, 255),
sf::Color(128, 255, 255),
sf::Color(255,128,255)
};