#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Player.hpp"
#include "Tile.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Stage.hpp"
#include "ScreenState.hpp"
#include "UI.hpp"

// Checks collision of all tiles with the player specifically

void checkTilePlayerCollision(std::vector<Tile>& tiles, Player& player, sf::Vector2f& direction)
{
    for (Tile& tile : tiles)
    {
        if (tile.getType() != 2 && tile.getCollider().checkCollision(player.getCollider(), direction, 1.0f))
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
    float push = 1.0f;
    if (enemy.getCanMove() == true) {
        push = 0.0f;
    }
    if (enemy.getCollider().checkCollision(player.getCollider(), direction, push))
    {
        if (push == 0.0f && enemy.getCanAttack()) 
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
    (void)font.openFromFile("assets/font/Hyper Oxide.ttf");

    unsigned int width = 1024;
    unsigned int height = 800;

    std::vector<String> stagestrings = {"./assets/stages/stage1.json","./assets/stages/stage2.json","./assets/stages/stage3.json","./assets/stages/stage4.json"};

    std::vector<Stage> stages;
    int current = 0;

    for (String stage : stagestrings) {
        stages.emplace_back(Stage(stage));
    }
    
    Stage* currentStage = &stages[current];
    std::vector<Tile> gametiles = currentStage->getTiles();

    std::vector<String> weaponstrings = {"./assets/ui-layout/weapon1.json","././assets/ui-layout/weapon2.json"};
    std::vector<String> healthstrings = {"./assets/ui-layout/5hp.json","./assets/ui-layout/4hp.json","./assets/ui-layout/3hp.json","./assets/ui-layout/2hp.json","./assets/ui-layout/1hp.json"};

    std::vector<UI> weapons;
    std::vector<UI> health;

    bool currentWeapon = 0;
    int currentHealth = 0;
    
    for (String ui : weaponstrings) {
        weapons.emplace_back(UI(ui));
    }

    for (String ui : healthstrings) {
        health.emplace_back(UI(ui));
    }

    UI* weaponTiles = &weapons[currentWeapon];
    UI* healthTiles = &health[currentHealth];

    std::vector<Tile> weapontiles = weaponTiles->getTiles();
    std::vector<Tile> healthtiles = healthTiles->getTiles();


    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode({width, height}), "Heartbleed");
    window->setFramerateLimit(60); // Limit frames to 60 FPS for convenience
 
    bool playChosen = showEndScreen(*window, font);
    if (!playChosen)
    {
        // User wants to exit, or closed window
        delete window; // Free the window
        return 0;      // End program
    }

    // initialize player
    Player player;
    player.setPosition(currentStage->getPlayerSpawn());
    std::vector<Bullet> bulletVec;
    std::vector<Enemy> enemies = currentStage->getEnemies();

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

        const float tolerance = 5.0f;
        sf::Vector2f playerPos = player.getPosition();
        sf::Vector2f goalPos = currentStage->getGoalPoint();
        
        if (std::abs(playerPos.x - goalPos.x) < tolerance &&
            std::abs(playerPos.y - goalPos.y) < tolerance)
        {
            // Load the next stage
            current++;
            currentStage = &stages[current];
            player.setPosition(currentStage->getPlayerSpawn());
            gametiles = currentStage->getTiles();

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
        enemy2.checkBullet(*player.getBullet());
        player.checkEnemyBullet(enemy2.getBullet(), enemy2.getBullet().getDamage());

        if (player.getHP() <= 0)
        {
            // Capture the last frame in 'lastFrame'
            auto winSize = window->getSize();
            sf::Vector2u textureSize(winSize.x, winSize.y);

            sf::Texture lastFrame(textureSize);
            // If you need sRGB false explicitly:
            // sf::Texture lastFrame(textureSize, false);

            lastFrame.update(*window);

            // Show the game over screen
            bool restart = showGameOver(*window, font, lastFrame);
            if (restart)
            {
                // Reset everything (e.g., health=100, stage=0, etc.)
                //player.setHP(100);
                //current = 0;
                //currentStage = Stage(stages[current]);
                //gametiles = currentStage.getTiles();
                //player.setPosition(currentStage.getPlayerSpawn());

                player.setHP(100);
                currentStage = &stages[0];
                gametiles = currentStage->getTiles();
                player.setPosition(currentStage->getPlayerSpawn());

                // Possibly reset enemies, bullets, etc.
                // ...
            }
            else
            {
                // Close or exit the game
                window->close();
                break; // or return 0;
            }
        }

        window->clear(sf::Color(0x3b3b3b));

        // Drawing shapes, sprites, etc.
        if (debugMode)
        {
            window->draw(debugText);
        }

        player.renderBullet(*window);
        player.render(*window);
        renderTiles(gametiles, window);
        renderTiles(weapontiles, window);
        renderTiles(healthtiles, window);


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
