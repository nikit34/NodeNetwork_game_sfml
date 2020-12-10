#include "Menu.h"


Menu::Menu(sf::RenderWindow& win) : window(win), end(false) { }

Menu::~Menu() { }


void Menu::run() {
    while (window.isOpen() && !end) {
        // poll any event to top in stack for follow proccessing
        while (window.pollEvent(event)) {

        }
        action();
        window.clear();
        draw();
        window.display();
    }
}


void Menu::action() {

}

void Menu::draw() {

}

void Menu::exit() {
    this->end = true;
}