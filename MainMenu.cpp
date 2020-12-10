#include "MainMenu.h"


MainMenu::MainMenu(sf::RenderWindow& window) : Menu(window), fullscreen(false) {

}

MainMenu::~MainMenu() { }


void MainMenu::draw() {
    window.draw(title);
}