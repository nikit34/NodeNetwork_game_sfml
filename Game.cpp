#include "Game.h"



Game::Game() :
	window(sf::VideoMode(800, 600), "SFML works!"), gboard(&players) {
    idPlayer = 1;
    players.push_back(Player(1, sf::Color(0, 255, 0)));
    players.push_back(Player(2, sf::Color::Red));
}

Game::~Game() { }


void Game::run() {
	this->gboard.init();
	this->dt = 1.f / 60.f;

    while (this->window.isOpen()) {
        sf::Event event;
        while (this->window.pollEvent(event)) {
            this->manageEvents(event);
        }

        this->gboard.update(this->dt);

        this->window.clear();
        this->window.draw(this->gboard);
        this->window.display();
    }
}

inline void Game::manageEvents(sf::Event event) {
    if (event.type == sf::Event::Closed)
        this->window.close();

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            this->window.close();
        }
        if (event.key.code == sf::Keyboard::Space) {
            this->gboard.randomize();
        }
    }

    // Pressed
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            this->posClickedMouse.x = event.mouseButton.x;
            this->posClickedMouse.y = event.mouseButton.y;
        }
        else if (event.mouseButton.button == sf::Mouse::Right) {
            this->posClickedMouse.x = event.mouseButton.x;
            this->posClickedMouse.y = event.mouseButton.y;

        }
    }

    // Released
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            this->proceed(this->posClickedMouse.x, this->posClickedMouse.y,
                event.mouseButton.x, event.mouseButton.y);
        }
        else if (event.mouseButton.button == sf::Mouse::Right) {
            if (this->gboard.getCloserCell(this->posClickedMouse.x, this->posClickedMouse.y)
                == this->gboard.getCloserCell(event.mouseButton.x, event.mouseButton.y))
            {
                this->gboard.free(this->idPlayer, this->gboard.getCloserCell(this->posClickedMouse.x,
                    this->posClickedMouse.y));
            }
            else {
                this->changeLinkState(this->posClickedMouse.x, this->posClickedMouse.y,
                    event.mouseButton.x, event.mouseButton.y);
            }
        }
    }
}

void Game::proceed(int orgX, int orgY, int destX, int destY) {
    int idOrg = this->gboard.getCloserCell(orgX, orgY);
    if (idOrg >= 0 && this->gboard.getOwner(idOrg) == this->idPlayer) {
        int idDest = this->gboard.getCloserCell(destX, destY);
        this->gboard.link(idOrg, idDest);
    }
}

void Game::changeLinkState(int orgX, int orgY, int destX, int destY) {
    int idOrg = this->gboard.getCloserCell(orgX, orgY);
    if (idOrg >= 0 && this->gboard.getOwner(idOrg) == this->idPlayer) {
        int idDest = this->gboard.getCloserCell(destX, destY);
        if (idDest >= 0) {
            this->gboard.changeLinkState(idOrg, idDest);
        }
    }
}
