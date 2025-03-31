#include "Game.hpp"
#include "Player.hpp"
#include "Tile.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Stage.hpp"
#include "ScreenState.hpp"
#include "UI.hpp"
#include "Boss.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <optional>
#include <string>
#include <sstream>
#include <iomanip>

void checkTilePlayerCollision(std::vector<Tile> &tiles, Player &player, sf::Vector2f &direction);
void checkTileEnemyCollision(std::vector<Tile> &tiles, Enemy &enemy, sf::Vector2f &direction);
void checkPlayerEnemyCollision(Player &player, Enemy &enemy, sf::Vector2f &direction);
void renderTiles(std::vector<Tile> &tiles, sf::RenderWindow *window);

Game::Game()
{
}

int Game::run()
{
    // --- Initialization ---
    sf::Music music;
    if (!music.openFromFile("assets/audio/temp2.wav"))
    {
        std::cerr << "Failed to load music\n";
    }
    music.play();

    sf::Font font;
    if (!font.openFromFile("assets/font/VCR.ttf"))
    {
        std::cerr << "Failed to load font\n";
    }

    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("assets/images/enemy/bullet-1-8.png"))
    {
        std::cerr << "Failed to load bullet textures\n";
    }

    unsigned int width = 1024;
    unsigned int height = 800;

    std::vector<std::string> stagestrings = {
        "./assets/stages/stage1.json",
        "./assets/stages/stage2.json",
        "./assets/stages/stage3.json",
        "./assets/stages/stage4.json"};

    std::cout << "pre-loading stages" << std::endl;

    std::vector<Stage> stages;
    std::vector<std::vector<Enemy>> stageEnemies;

    int current = 0;
    for (const String &stage : stagestrings)
    {
        stages.emplace_back(Stage(stage));
    }
    for (auto stage : stages)
    {
        stageEnemies.emplace_back(stage.getEnemies());
    }

    Stage *currentStage = &stages[current];

    std::vector<Tile> gametiles = currentStage->getTiles();

    std::cout << "stage loading complete" << std::endl;

    std::vector<std::string> weaponstrings = {
        "./assets/ui-layout/weapon1.json",
        "./assets/ui-layout/weapon2.json"};

    std::cout << "weapon loading complete" << std::endl;

    std::vector<std::string> healthstrings = {
        "./assets/ui-layout/1hp.json",
        "./assets/ui-layout/2hp.json",
        "./assets/ui-layout/3hp.json",
        "./assets/ui-layout/4hp.json",
        "./assets/ui-layout/5hp.json"};

    std::cout << "ui loading complete" << std::endl;

    std::vector<UI> weapon;
    std::vector<UI> health;
    for (const String &ui : weaponstrings)
    {
        weapon.emplace_back(UI(ui));
    }
    for (const String &ui : healthstrings)
    {
        health.emplace_back(UI(ui));
    }
    UI weaponTiles = weapon[0];
    UI healthTiles = health[0];

    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode({width, height}), "Heartbleed");
    window->setFramerateLimit(60); // Limit frames to 60 FPS

    bool hardModeEnabled = false;

    MenuResult menuResult = showMenu(*window, font);
    if (menuResult == MenuResult::Exit)
    {
        delete window;
        return 0;
    }
    else if (menuResult == MenuResult::HardMode)
    {
        hardModeEnabled = true;
    }

    // --- Game Objects ---
    Player player;
    player.setPosition(currentStage->getPlayerSpawn());
    std::vector<Bullet> bulletVec;
    for (Enemy &enemy : stageEnemies[current])
    {
        enemy.printStatus();
    }

    /*     Enemy enemy(sf::Vector2f(64, 64), sf::Color::Red, true);
        Enemy enemy2(sf::Vector2f(64, 64), sf::Color::Red, false);

        enemy.setPos(sf::Vector2f(350, 331));
        enemy.setDetectionRange(250.f);
        enemy.setSpeed(100.f);

        enemy2.setPos(sf::Vector2f(420, 331));
        enemy2.setDetectionRange(250.f);
        enemy2.setDirection(-1.f); */

    Boss boss;
    boss.setPos({704, 400});
    boss.setDirection(-1.f);

    sf::Text debugText(font);
    debugText.setCharacterSize(20);
    debugText.setFillColor(sf::Color::White);
    debugText.setPosition(sf::Vector2f(640.f, 10.f));

    sf::Text timerText(font);
    timerText.setCharacterSize(32);
    timerText.setFillColor(sf::Color::Green);
    timerText.setPosition(sf::Vector2f(96.f, 760.f));

    sf::Text stageTitle(font);
    stageTitle.setCharacterSize(46);
    stageTitle.setFillColor(sf::Color::Green);
    stageTitle.setPosition(sf::Vector2f(96.f, 22.f));
    stageTitle.setString(currentStage->getName());

    sf::Clock gameClock;
    sf::Clock deltaClock;
    bool debugMode = false;

    // --- Main Game Loop ---
    while (window->isOpen())
    {
        sf::Time dt = deltaClock.restart();

        while (const std::optional event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                music.stop();
                window->close();
            }
            else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                player.handleInput(keyPressed->scancode, true, *window);

                if (keyPressed->scancode == sf::Keyboard::Scancode::B)
                {
                    debugMode = !debugMode;
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                {
                    auto winSize = window->getSize();
                    sf::Vector2u textureSize(winSize.x, winSize.y);

                    sf::Texture lastFrame(textureSize);

                    lastFrame.update(*window);
                    music.pause();
                    gameClock.stop();
                    MenuResult menuResult = showPaused(*window, font, lastFrame, gameClock.getElapsedTime(), music, player);
                    if (menuResult == MenuResult::Play)
                    {
                        gameClock.start();
                        deltaClock.restart();
                        player.resetDeltaTime();
                        music.play();
                        continue;
                    }
                    else if (menuResult == MenuResult::Exit)
                    {
                        delete window;
                        return 0;
                    }
                }
            }
            // Stop input
            else if (const auto *keyPressed = event->getIf<sf::Event::KeyReleased>())
            {
                player.handleInput(keyPressed->scancode, false, *window);
            }
        }

        // Update game objects
        player.update();

        for (Enemy &enemy : stageEnemies[current])
        {
            // 1. Update enemy logic
            enemy.update(dt, player.getPosition());
        }

        // enemy.update(dt, player.getPosition());
        // enemy2.update(dt, player.getPosition());

        if (current == 3)
        {
            boss.update(dt);
        }

        const float tolerance = 16.0f;
        sf::Vector2f playerPos = player.getCollider().getPosition();
        sf::Vector2f goalPos = currentStage->getGoalPoint();

        if (std::abs(playerPos.x - goalPos.x) < tolerance &&
            std::abs(playerPos.y - goalPos.y) < tolerance)
        {
            // Load the next stage
            current++;
            if (player.getHP() < 5)
            {
                player.setHP(player.getHP() + 1);
            }
            currentStage = &stages[current];
            player.setPosition(currentStage->getPlayerSpawn());
            gametiles = currentStage->getTiles();
            stageTitle.setString(currentStage->getName());

            for (Enemy &enemy : stageEnemies[current])
            {
                enemy.printStatus();
            }
        }

        /*         if (player.getMode()) {
                    enemy.setColor(sf::Color::Red);
                } else {
                    enemy.setColor(sf::Color::White);
                } */

        if (debugMode)
        {
            int fps = static_cast<int>(1 / dt.asSeconds());
            debugText.setString("Fps: " + std::to_string(fps) +
                                "\nPlayer Coords: x: " + std::to_string((int)player.getPosition().x) +
                                ", y: " + std::to_string((int)player.getPosition().y) +
                                "\nPlayer Block: x: " + std::to_string(static_cast<int>((player.getPosition().x / 32) - 3)) +
                                ", y: " + std::to_string(static_cast<int>((player.getPosition().y / 32) - 3)));
        }

        sf::Vector2f direction;
        // sf::Vector2f enemyDirection;
        sf::Vector2f enemyDirection2;

        checkTilePlayerCollision(gametiles, player, direction);

        // checkTileEnemyCollision(gametiles, boss, enemyDirection2);
        // checkPlayerEnemyCollision(player, boss, direction);

        if (current == 3)
        {
            boss.checkBullet(*player.getBullet());
            player.checkEnemyBullet(boss.getBullet(), boss.getDamage());
            player.checkEnemyBullet(boss.getOtherBullet(), boss.getDamage());
        }

        for (Enemy &enemy : stageEnemies[current])
        {
            sf::Vector2f enemyDirection;

            checkTileEnemyCollision(gametiles, enemy, enemyDirection);
            checkPlayerEnemyCollision(player, enemy, direction);

            enemy.checkBullet(*player.getBullet());

            player.checkEnemyBullet(enemy.getBullet(), enemy.getDamage());
        }

        // checkTileEnemyCollision(gametiles, enemy, enemyDirection);
        // checkTileEnemyCollision(gametiles, enemy2, enemyDirection2);

        // checkPlayerEnemyCollision(player, enemy, direction);
        // enemy.checkBullet(*player.getBullet());
        // enemy2.checkBullet(*player.getBullet());
        // player.checkEnemyBullet(enemy2.getBullet(), enemy2.getBullet().getDamage());

        if (player.getHP() <= 0)
        {
            auto winSize = window->getSize();
            sf::Vector2u textureSize(winSize.x, winSize.y);
            sf::Texture lastFrame(textureSize);
            lastFrame.update(*window);
            bool restart = showGameOver(*window, font, lastFrame);
            if (restart)
            {
                player.setHP(5);
                current = 0;
                currentStage = &stages[0];
                gametiles = currentStage->getTiles();
                player.setPosition(currentStage->getPlayerSpawn());
            }
            else
            {
                window->close();
                break;
            }
        }

        /*         if (player.getPosition().x > 300.0f)
                {
                    bool playAgain = showEndScreen(*window, font, gameClock.getElapsedTime());

                    if (playAgain)
                    {
                        window->close();
                        break;
                    }
                    else
                    {
                        window->close();
                        break;
                    }
                } */

        window->clear(sf::Color(16, 36, 29, 255));

        sf::Time elapsed = gameClock.getElapsedTime();
        long totalMs = elapsed.asMilliseconds();

        int minutes = totalMs / (60 * 1000);
        int seconds = (totalMs % (60 * 1000)) / 1000;
        int centiseconds = (totalMs % 1000) / 10;

        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << minutes << ":"
            << std::setw(2) << std::setfill('0') << seconds << ":"
            << std::setw(2) << std::setfill('0') << centiseconds;

        timerText.setString("Time: " + oss.str());

        window->draw(timerText);
        window->draw(stageTitle);

        if (debugMode)
        {
            window->draw(debugText);
            player.setHP(5);
        }

        player.renderBullet(*window);
        player.render(*window);
        renderTiles(gametiles, window);
        renderTiles(health[player.getHP() - 1].getTiles(), window);
        renderTiles(weapon[player.getBulletType() - 1].getTiles(), window);

        // enemy.draw(*window);
        // enemy2.drawBullet(*window);
        // enemy2.draw(*window);

        if (current == 3)
        {
            boss.drawBullet(*window);
            boss.drawVBullet(*window);
            boss.draw(*window);
        }

        for (Enemy &enemy : stageEnemies[current])
        {

            enemy.setNotWall();
            enemy.draw(*window);
            enemy.getBullet().setTexture(bulletTexture);
            enemy.drawBullet(*window);
        }

        window->display();
    }

    music.stop();
    delete window;
    return 0;
}

void checkTilePlayerCollision(std::vector<Tile> &tiles, Player &player, sf::Vector2f &direction)
{
    for (Tile &tile : tiles)
    {
        if (tile.getType() != 2 && tile.getCollider().checkCollision(player.getCollider(), direction, 1.0f))
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
    float push = 1.0f;

    if (enemy.getCanMove() == true)
    {
        push = 0.0f;
    }
    if (enemy.getCollider().checkCollision(player.getCollider(), direction, push))
    {
        if (push == 0.0f && !enemy.getCanMove())
        {
            enemy.disableAttack();
            player.takeDamage(enemy.getDamage());
        }
        else
        {
            player.onCollision(direction);
        }
    }
}

void renderTiles(std::vector<Tile> &tiles, sf::RenderWindow *window)
{
    for (Tile &tile : tiles)
    {
        tile.render(*window);
    }
}