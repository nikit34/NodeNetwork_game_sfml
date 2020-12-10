#include "Game.h"



Game::Game() :
	window(sf::VideoMode(800, 600), "SFML works!"), gboard(&players) {

}

Game::~Game() { }


void Game::run() {
	this->gboard.init();
}