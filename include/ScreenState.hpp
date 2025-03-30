#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

enum class MenuResult
{
    Play,
    HardMode,
    Exit
};

MenuResult showMenu(sf::RenderWindow &window, sf::Font &font);
bool showGameOver(sf::RenderWindow &window, sf::Font &font, const sf::Texture &backgroundTexture);
bool showEndScreen(sf::RenderWindow &window, sf::Font &font, sf::Time timer);

#endif // MENU_HPP
