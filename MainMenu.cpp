#include "MainMenu.h"


MainMenu::MainMenu(sf::RenderWindow& window) : Menu(window), fullscreen(false) {
    this->settings.antialiasingLevel = 10; // сглаживание текста

    this->window.create(
        sf::VideoMode(800, 600), 
        "NodeNetwork", 
        sf::Style::Default, 
        this->settings
    );

    this->window.setFramerateLimit(60); // fps
    if (this->icon.loadFromFile("Textures/icon.png"))
        window.setIcon(256, 256, this->icon.getPixelsPtr());

    this->font.loadFromFile("Textures/Symtext.ttf");

    this->title = sf::Text("NodeNetwork", this->font, 46);
    this->title.setFillColor(sf::Color(255, 0, 0));
    this->centerTextXaxis(this->title, 50.f);


    this->maintext = sf::Text("Server", this->font, 32);
    this->option.resize(4);
    this->option[0].txt = this->maintext;
    // this shit take from book - move address of function to field struct of array 
    this->option[0].f = static_cast<void(Menu::*)()>(&MainMenu::server);

    this->maintext.setString("Client");
    this->option[1].txt = this->maintext;
    this->option[1].f = static_cast<void(Menu::*)()>(&MainMenu::client);

    this->maintext.setString("Mode");
    this->option[2].txt = this->maintext;
    this->option[2].txt.setString("Mode : windowed");
    this->option[2].f = static_cast<void(Menu::*)()>(&MainMenu::mode);

    this->maintext.setString("Exit");
    this->option[3].txt = this->maintext;
    this->option[3].f = &Menu::exit;

    this->calculateTextYaxis();
}

MainMenu::~MainMenu() { }


void MainMenu::draw() {
    this->window.draw(this->title);
}


void MainMenu::centerTextXaxis(sf::Text& txt, float y) {
    txt.setPosition(
        (this->window.getSize().x - txt.getGlobalBounds().width) / 2.f,
        y
    );
}


void MainMenu::calculateTextYaxis() {
    for (uint16_t i = 0; i < 4; ++i) {
        this->centerTextXaxis(this->option[i].txt, 150.f + i * 100.f);
    }
}

void MainMenu::server() {

}

void MainMenu::client() {

}

void MainMenu::mode() {
  
}
