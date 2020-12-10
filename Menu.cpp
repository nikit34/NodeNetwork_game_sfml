#include "Menu.h"


Menu::Menu(sf::RenderWindow& win): window(win), end(false) { }

Menu::~Menu() { }

void Menu::centerTextXaxis(sf::Text& txt, float y) {
    txt.setPosition(
        (this->window.getSize().x - txt.getGlobalBounds().width) / 2.f, 
        y
    );
}


void Menu::run() {
    while (this->window.isOpen() && !this->end) {
        // poll any event to top in stack for follow proccessing
        while (this->window.pollEvent(this->event)) {
            // user wants to close the window
            if (this->event.type == sf::Event::Closed)
                this->window.close();
            
            if (this->event.type == sf::Event::KeyPressed) {
                if (this->event.key.code == sf::Keyboard::Escape) {
                    this->end = true;
                }
            }
        }
        this->action();
        this->window.clear();
        this->draw();
        this->window.display();
    }
}


void Menu::action() {

}

void Menu::draw() {

}

void Menu::exit() {
    this->end = true;
}