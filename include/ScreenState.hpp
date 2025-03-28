#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <optional>

bool showMenu(sf::RenderWindow &window, sf::Font &font);
bool showGameOver(sf::RenderWindow &window, sf::Font &font, const sf::Texture &backgroundTexture);
bool showEndScreen(sf::RenderWindow &window, sf::Font &font);

#endif // MENU_HPP
