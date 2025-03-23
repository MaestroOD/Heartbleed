#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Player.hpp"
#include "Tile.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"

// Checks collision of all tiles with the player specifically
void checkTilePlayerCollision(std::vector<Tile> &tiles, Player &player, sf::Vector2f &direction)
{
    for (Tile &tile : tiles)
    {
        if (tile.getCollider().checkCollision(player.getCollider(), direction, 1.0f))
        {
            player.onCollision(direction);
        }
    }
}

// Draws all tiles
void renderTiles(std::vector<Tile> &tiles, sf::RenderWindow *window)
{
    for (Tile &tile : tiles)
    {
        tile.render(*window);
    }
}

int main()
{

    unsigned int width = 640;
    unsigned int height = 360;
    unsigned int currentStage = 0;



    std::vector<std::vector<Tile>> gametiles = generateWorld();

    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode({width, height}), "Prototype");
    window->setFramerateLimit(60); // Limit frames to 60 FPS for convenience
    Player player;
    Enemy enemy(sf::Vector2f(64, 64), sf::Color::Red, true);
    std::vector<Bullet> bulletVec;

    enemy.setPos(sf::Vector2f(350, 231));

    // Tiles and shid
    //Tile wall1(nullptr, Vector2f(100.0f, 300.0f), Vector2f(500.0f, 180.0f), 0);    // (Texture path, size, pos, type), wall
    //Tile platform1(nullptr, Vector2f(600.0f, 50.0f), Vector2f(320.0f, 320.0f), 1); // Ground
    //Tile platform2(nullptr, Vector2f(50.0f, 5.0f), Vector2f(320.0f, 200.0f), 0);   // Random platform
    //std::vector<Tile> tiles;
    //tiles.push_back(wall1);
    //tiles.push_back(platform1);
    //tiles.push_back(platform2);
    // Add all tiles to an array, and make a function that goes through the array and checks collision with player
    // Do same thing with enemies except enemies will also be in array, will run array check for tiles for each enemy
    // Make sure to start a thread for each check

    while (window->isOpen())
    {
        while (const std::optional event = window->pollEvent())
        {
            // Close when x button is pressed
            if (event->is<sf::Event::Closed>())
            {
                window->close();
            }
            // Check keyboard inputs
            else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                player.handleInput(keyPressed->scancode, true, *window);
            }
            // Stop input
            else if (const auto *keyPressed = event->getIf<sf::Event::KeyReleased>())
            {
                player.handleInput(keyPressed->scancode, false, *window);
            }
        }

        // Render (collision checks go here)
        player.update();
        if (player.getPosition().x > 1000.0f) 
        {
            // Load the next stage
            currentStage++;
			player.setPosition(100.0f, 100.0f);
        } 

        if (player.getMode())
        {
            enemy.setColor(sf::Color::Blue);
        }
        else
        {
            enemy.setColor(sf::Color::Red);
        }
        sf::Vector2f direction;

        checkTilePlayerCollision(gametiles.at(currentStage), player, direction);
        enemy.checkBullet(player.getBullet());

        window->clear(sf::Color(0xFF8800FF));

        // Drawing shapes, sprites, etc.
        player.render(*window);
        player.renderBullet(*window);
        renderTiles(gametiles.at(currentStage), window, texture);
        enemy.draw(*window);

        window->display();
    }

    delete window;
    return 0;
}
