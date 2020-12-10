#pragma once

#include "Menu.h"


class MainMenu : public Menu {
public:
	MainMenu(sf::RenderWindow& win);
	virtual void run();
	virtual ~MainMenu();
	
	void exit();

protected:
	virtual void draw();
	bool fullscreen;
};