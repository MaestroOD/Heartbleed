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

    sf::Font font;
    if (!font.openFromFile("assets/font/Roboto-Thin.ttf"))
    {
        std::cerr << "Error loading font!" << std::endl;
    }

    sf::Music music;
    if (!music.openFromFile("assets/audio/temp.wav"))
    {
        std::cerr << "Failed to load music\n";
    }
    music.play();
    unsigned int width = 1000;
    unsigned int height = 800;
    unsigned int currentStage = 0;

    std::vector<std::vector<Tile>> gametiles = generateWorld();

    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode({width, height}), "Prototype");
    window->setFramerateLimit(60); // Limit frames to 60 FPS for convenience
    Player player;
    Enemy enemy(sf::Vector2f(64, 64), sf::Color::Red, true);
    std::vector<Bullet> bulletVec;

    enemy.setPos(sf::Vector2f(350, 231));
    enemy.setDetectionRange(250.f);
    enemy.setSpeed(100.f);

    sf::Text fpsText(font);
    fpsText.setCharacterSize(20);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition(sf::Vector2f(50.f, 10.f));

    sf::Clock deltaClock;
    bool showFPS = false;

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
        float deltaTime = deltaClock.restart().asSeconds();

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
                    showFPS = !showFPS;
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
        enemy.update(deltaTime, player.getPosition());

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

        if (showFPS)
        {
            int fps = (1 / deltaTime);
            fpsText.setString("FPS: " + std::to_string(fps));
        }

        sf::Vector2f direction;
        sf::Vector2f enemyDirection;

        checkTilePlayerCollision(gametiles.at(currentStage), player, direction);
        checkTileEnemyCollision(gametiles.at(currentStage), enemy, enemyDirection);
        enemy.checkBullet(player.getBullet());

        window->clear(sf::Color(0xFF8800FF));

        // Drawing shapes, sprites, etc.
        if (showFPS)
        {
            window->draw(fpsText);
        }
        player.renderBullet(*window);
        player.render(*window);
        renderTiles(gametiles.at(currentStage), window);
        enemy.draw(*window);
        window->display();
    }

    delete window;
    return 0;
}
