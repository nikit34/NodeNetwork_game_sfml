#include <iostream>
#include <ctime>
#include <cstdlib>

#include "MainMenu.h"



int main(){
    srand(static_cast<unsigned int>(time(0)));
    sf::RenderWindow window(sf::VideoMode(400, 300), "NodeNetwork", sf::Style::None);
    MainMenu menu(window);
    menu.run();

    return 0;
}