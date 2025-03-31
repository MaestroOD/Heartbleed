#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <optional>
#include "Player.hpp"

enum class MenuResult
{
    Play,
    HardMode,
    Exit,
    Paused
};

MenuResult showMenu(sf::RenderWindow &window, sf::Font &font);
MenuResult showPaused(sf::RenderWindow &window, sf::Font &font, const sf::Texture &backgroundTexture, sf::Time timer, sf::Music &music, Player &player);
MenuResult showSettings(sf::RenderWindow &window, sf::Font &font, sf::Music &music, Player &player);
bool showGameOver(sf::RenderWindow &window, sf::Font &font, sf::Time timer);
bool showEndScreen(sf::RenderWindow &window, sf::Font &font, sf::Time timer);

#endif // MENU_HPP
