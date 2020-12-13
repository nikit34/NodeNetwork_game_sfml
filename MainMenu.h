#pragma once

#include "Menu.h"
#include "Server.h"
#include "Client.h"



class MainMenu : public Menu {
public:
	MainMenu(sf::RenderWindow& windows);
	virtual ~MainMenu();
	
	void server();
	void client();
	void mode();

protected:
	virtual void draw();
	void centerTextXaxis(sf::Text& txt, float&& y);
	void calculateTextYaxis(float&& y);

	bool fullscreen;
	sf::Text title;
	sf::Text maintext;
	sf::Image icon;

private:
	sf::ContextSettings settings;
};