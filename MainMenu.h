#pragma once

#include "Menu.h"


class MainMenu : public Menu {
public:
	MainMenu(sf::RenderWindow& windows);
	virtual ~MainMenu();
	

protected:
	virtual void draw();

	bool fullscreen;
	sf::Text title;
};