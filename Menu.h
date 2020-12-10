#pragma once

#include <SFML/Graphics.hpp>



class Menu {
public:
	Menu(sf::RenderWindow& window);
	virtual ~Menu();
	
	virtual void run(); 
	void exit();

protected:
	virtual void action();
	virtual void draw();

	sf::RenderWindow& window;
	sf::Event event;

	bool end;
};