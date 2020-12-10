#pragma once

#include <SFML/Graphics.hpp>


class Menu;
struct Button {
	sf::Text txt;
	void (Menu::* f)(); // point to abstract button
};

class Menu {
public:
	Menu(sf::RenderWindow& window);
	virtual ~Menu();
	
	virtual void run(); 
	void exit();

protected:
	virtual void action();
	virtual void draw();
	void centerTextXaxis(sf::Text& txt, float y);

	sf::RenderWindow& window;
	sf::Event event;
	std::vector<Button> option;
	sf::Vector2f click;

private:
	bool end;
};