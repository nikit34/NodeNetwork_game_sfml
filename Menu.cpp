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

            if (this->event.type == sf::Event::MouseButtonPressed) {
                if (this->event.mouseButton.button == sf::Mouse::Left) {
                    this->click = this->window.mapPixelToCoords(
                        sf::Mouse::getPosition(this->window)
                    );
                }
            }
            else if (this->event.type == sf::Event::MouseButtonReleased) {
                for (uint16_t i = 0; i < this->option.size(); ++i) {
                    if (this->option[i].txt.getGlobalBounds().contains(
                            this->window.mapPixelToCoords(
                                sf::Mouse::getPosition(this->window)
                            )
                        )
                        && this->option[i].txt.getGlobalBounds().contains(
                                this->click
                            )
                        ) {
                        if (this->option[i].f != nullptr) {
                            (this->*option[i].f)();
                        }
                    }
                }
            }
        }
        this->action();
        this->window.clear();

        for (uint16_t i = 0; i < this->option.size(); ++i) {
            if (this->option[i].txt.getGlobalBounds().contains(
                    this->window.mapPixelToCoords(
                        sf::Mouse::getPosition(window)
                    )
            )) {
                this->option[i].txt.setFillColor(sf::Color(255, 100, 100));
            }
            else {
                this->option[i].txt.setFillColor(sf::Color(255, 255, 255));
            }
            this->window.draw(this->option[i].txt);
        }

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