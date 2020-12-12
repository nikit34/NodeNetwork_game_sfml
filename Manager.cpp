#include "Manager.h"



FontManager::FontManager() {}

std::map<std::string, sf::Font*> FontManager::fonts;


sf::Font* FontManager::getFont(std::string str) {
    if (fonts.find(str) != fonts.end())
        return fonts[str];

    sf::Font* font = new sf::Font();
    if (font->loadFromFile(str)) {
        fonts[str] = font;
        return fonts[str];
    }
    else {
        delete font;
        return nullptr;
    }

}

FontManager::~FontManager() {
    sf::Font* font;
    std::map<std::string, sf::Font*>::iterator iter = fonts.begin();

    while (iter != fonts.end()) {
        font = iter->second;
        delete font;
        ++iter;
    }

}
