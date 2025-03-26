#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Player.hpp"
#include "Tile.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "World.hpp"

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

void checkTileEnemyCollision(std::vector<Tile> &tiles, Enemy &enemy, sf::Vector2f &direction)
{
    for (Tile &tile : tiles)
    {
        if (tile.getCollider().checkCollision(enemy.getCollider(), direction, 1.0f))
        {
            enemy.onCollision(direction);
        }
    }
}

void checkPlayerEnemyCollision(Player &player, Enemy &enemy, sf::Vector2f &direction)
{
    if (enemy.getCollider().checkCollision(player.getCollider(), direction, 1.0f) && enemy.getCanAttack())
    {
        enemy.disableAttack();
        player.takeDamage(enemy.getDamage());
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
    // sf::Music music;
    // if (!music.openFromFile("assets/audio/temp.wav"))
    //{
    // std::cerr << "Failed to load music\n";
    //}
    // music.play();

    sf::Font font;
    (void)font.openFromFile("assets/font/Penis Typography Black.ttf");

    unsigned int width = 1024;
    unsigned int height = 800;
    unsigned int currentStage = 0;

    std::vector<std::vector<Tile>> gametiles = generateWorld();

    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode({width, height}), "Prototype");
    window->setFramerateLimit(60); // Limit frames to 60 FPS for convenience
    Player player;
    Enemy enemy(sf::Vector2f(64, 64), sf::Color::Red, true); // normal foolow
    Enemy enemy2(sf::Vector2f(64, 64), sf::Color::Red, false);
    std::vector<Bullet> bulletVec;

    enemy.setPos(sf::Vector2f(350, 231));
    enemy.setDetectionRange(250.f);
    enemy.setSpeed(100.f);

    enemy2.setPos(sf::Vector2f(390, 231));
    enemy2.setDetectionRange(250.f);
    enemy2.setDirection(-1.f); // -1 = left, 1 = right

    sf::Text debugText(font);
    debugText.setCharacterSize(20);
    debugText.setFillColor(sf::Color::White);
    debugText.setPosition(sf::Vector2f(50.f, 10.f));

    sf::Clock deltaClock;
    bool debugMode = false;

    // Tiles and shid
    // Tile wall1(nullptr, Vector2f(100.0f, 300.0f), Vector2f(500.0f, 180.0f), 0);    // (Texture path, size, pos, type), wall
    // Tile platform1(nullptr, Vector2f(600.0f, 50.0f), Vector2f(320.0f, 320.0f), 1); // Ground
    // Tile platform2(nullptr, Vector2f(50.0f, 5.0f), Vector2f(320.0f, 200.0f), 0);   // Random platform
    // std::vector<Tile> tiles;
    // tiles.push_back(wall1);
    // tiles.push_back(platform1);
    // tiles.push_back(platform2);
    // Add all tiles to an array, and make a function that goes through the array and checks collision with player
    // Do same thing with enemies except enemies will also be in array, will run array check for tiles for each enemy
    // Make sure to start a thread for each check

    while (window->isOpen())
    {
        Time dt = deltaClock.restart();

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

                if (keyPressed->scancode == sf::Keyboard::Scancode::B)
                {
                    debugMode = !debugMode;
                }
            }
            // Stop input
            else if (const auto *keyPressed = event->getIf<sf::Event::KeyReleased>())
            {
                player.handleInput(keyPressed->scancode, false, *window);
            }
        }

        // Render (collision checks go here)
        player.update();
        enemy.update(dt, player.getPosition());
        enemy2.update(dt, player.getPosition());

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

        if (debugMode)
        {
            int fps = (1 / dt.asSeconds());
            debugText.setString("Fps: " + std::to_string(fps) + "\nPlayer Coords: x: " + std::to_string(player.getPosition().x) + ", y: " + std::to_string(player.getPosition().y));
        }

        sf::Vector2f direction;
        sf::Vector2f enemyDirection;
        sf::Vector2f enemyDirection2;

        checkTilePlayerCollision(gametiles.at(currentStage), player, direction);
        checkTileEnemyCollision(gametiles.at(currentStage), enemy, enemyDirection);
        checkTileEnemyCollision(gametiles.at(currentStage), enemy2, enemyDirection2);
        checkPlayerEnemyCollision(player, enemy, direction);
        enemy.checkBullet(*player.getBullet());
        player.checkEnemyBullet(enemy2.getBullet(), enemy2.getDamage());

        window->clear(sf::Color(0xFF8800FF));

        // Drawing shapes, sprites, etc.
        if (debugMode)
        {
            window->draw(debugText);
        }

        player.renderBullet(*window);
        player.render(*window);
        renderTiles(gametiles.at(currentStage), window);
        enemy.draw(*window);
        enemy2.drawBullet(*window);
        enemy2.draw(*window);
        window->display();
    }

    delete window;
    return 0;
}
