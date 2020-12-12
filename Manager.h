#pragma once
#include<iostream>
#include<map>

#include "SFML/Graphics.hpp"



class FontManager {
private:
	static std::map<std::string, sf::Font*> fonts;
	FontManager();

public:
	~FontManager();
	static sf::Font* getFont(std::string str);
};
