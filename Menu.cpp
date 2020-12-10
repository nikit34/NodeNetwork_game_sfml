#include "Menu.h"


Menu::Menu(sf::RenderWindow& win) : window(win), end(false) { }

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