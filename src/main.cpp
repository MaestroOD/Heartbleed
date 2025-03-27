#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Player.hpp"
#include "Tile.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Stage.hpp"

// Checks collision of all tiles with the player specifically

void checkTilePlayerCollision(std::vector<Tile>& tiles, Player& player, sf::Vector2f& direction)
{
    for (Tile& tile : tiles)
    {
        if (tile.getCollider().checkCollision(player.getCollider(), direction, 1.0f))
        {
            player.onCollision(direction);
        }
    }
}

void checkTileEnemyCollision(std::vector<Tile>& tiles, Enemy& enemy, sf::Vector2f& direction)
{
    for (Tile& tile : tiles)
    {
        if (tile.getCollider().checkCollision(enemy.getCollider(), direction, 1.0f))
        {
            enemy.onCollision(direction);
        }
    }
}

void checkPlayerEnemyCollision(Player& player, Enemy& enemy, sf::Vector2f& direction)
{
    if (enemy.getCollider().checkCollision(player.getCollider(), direction, 1.0f) && enemy.getCanAttack())
    {
        enemy.disableAttack();
        player.takeDamage(enemy.getDamage());
    }
}

// Draws all tiles
void renderTiles(std::vector<Tile>& tiles, sf::RenderWindow* window)
{
    for (Tile& tile : tiles)
    {
        tile.render(*window);
    }
}

int main()
{
    sf::Music music;
    if (!music.openFromFile("assets/audio/temp2.wav"))
        {
        std::cerr << "Failed to load music\n";
        }
    music.play();

    sf::Font font;
    (void)font.openFromFile("assets/font/Penis Typography Black.ttf");

    unsigned int width = 1024;
    unsigned int height = 800;

    Stage currentStage = Stage("./assets/stages/stage1.json");
    std::vector<Tile> gametiles = currentStage.getTiles();

    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode({width, height}), "Heartbleed");
    window->setFramerateLimit(60); // Limit frames to 60 FPS for convenience

    // initialize player
    Player player;
    player.setPosition(currentStage.getPlayerSpawn());
    std::vector<Bullet> bulletVec;
    //std::vector<Enemy> enemies = currentStage.getEnemies();

    Enemy enemy(sf::Vector2f(64, 64), sf::Color::Red, true); // normal foolow
    Enemy enemy2(sf::Vector2f(64, 64), sf::Color::Red, false);

    enemy.setPos(sf::Vector2f(350, 331));
    enemy.setDetectionRange(250.f);
    enemy.setSpeed(100.f);

    enemy2.setPos(sf::Vector2f(420, 331));
    enemy2.setDetectionRange(250.f);
    enemy2.setDirection(-1.f); // -1 = left, 1 = right

    sf::Text debugText(font);
    debugText.setCharacterSize(20);
    debugText.setFillColor(sf::Color::White);
    debugText.setPosition(sf::Vector2f(50.f, 10.f));

    sf::Clock deltaClock;
    bool debugMode = false;

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
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                player.handleInput(keyPressed->scancode, true, *window);

                if (keyPressed->scancode == sf::Keyboard::Scancode::B)
                {
                    debugMode = !debugMode;
                }
            }
            // Stop input
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyReleased>())
            {
                player.handleInput(keyPressed->scancode, false, *window);
            }
        }

        // Render (collision checks go here)
        player.update();
        enemy.update(dt, player.getPosition());
        enemy2.update(dt, player.getPosition());

  /*       for (auto& enemy : currentStage.getEnemies()) {
            enemy.update(dt, player.getPosition());
        } */

        if (player.getPosition().x > 1000.0f)
        {
            // Load the next stage
            currentStage;
            player.setPosition(currentStage.getPlayerSpawn());
        }

        if (player.getMode())
        {
            enemy.setColor(sf::Color::Red);
        }
        else
        {
            enemy.setColor(sf::Color::White);
        }

        if (debugMode)
        {
            int fps = (1 / dt.asSeconds());
            debugText.setString("Fps: " + std::to_string(fps) + 
            "\nPlayer Coords: x: " + std::to_string(player.getPosition().x) + 
            ", y: " + std::to_string(player.getPosition().y) + 
            "\nPlayer Block: x: " + std::to_string((int)((player.getPosition().x / 32)-3)) +
            ", y: " + std::to_string((int)((player.getPosition().y / 32)-3)));
        }

        sf::Vector2f direction;
        sf::Vector2f enemyDirection;
        sf::Vector2f enemyDirection2;

        checkTilePlayerCollision(gametiles, player, direction);

/*         for (auto& enemy : currentStage.getEnemies()) {
            checkTileEnemyCollision(gametiles, enemy, enemyDirection);
            checkPlayerEnemyCollision(player, enemy, direction);
            enemy.checkBullet(*player.getBullet());
            player.checkEnemyBullet(enemy.getBullet(), enemy.getDamage());

        }  */


        checkTileEnemyCollision(gametiles, enemy, enemyDirection);
        checkTileEnemyCollision(gametiles, enemy2, enemyDirection2);

        checkPlayerEnemyCollision(player, enemy, direction);
        enemy.checkBullet(*player.getBullet());
        player.checkEnemyBullet(enemy2.getBullet(), enemy2.getDamage());

        window->clear(sf::Color(0x3b3b3b));

        // Drawing shapes, sprites, etc.
        if (debugMode)
        {
            window->draw(debugText);
        }

        player.renderBullet(*window);
        player.render(*window);
        renderTiles(gametiles, window);

/*         for (auto& enemy : currentStage.getEnemies()) {
            enemy.draw(*window);
            enemy.drawBullet(*window);
        }  */
       
        enemy.draw(*window);
        enemy2.drawBullet(*window);
        enemy2.draw(*window);
        window->display();
    }

    delete window;
    return 0;
}
