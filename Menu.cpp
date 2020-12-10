#include "Menu.h"


Menu::Menu(sf::RenderWindow& win) : win(win), end(false) { }

Menu::~Menu() { }


void Menu::run() {

}

void Menu::action() {

}

void Menu::draw() {

}

void Menu::exit() {
    this->end = true;
}