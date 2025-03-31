#include "ScreenState.hpp"

MenuResult showMenu(sf::RenderWindow &window, sf::Font &font)
{

    sf::Texture backgroundTexture;
    sf::Texture borderTexture; // I fucking hate this shit, sfml just doesn't like the name greenBorderTexture
    sf::Texture titleTexture;
    sf::Texture playTexture;
    sf::Texture hardTexture;
    sf::Texture exitTexture;

    (void)backgroundTexture.loadFromFile("assets/images/title-screen/title-bg.png");
    (void)borderTexture.loadFromFile("assets/images/title-screen/title-box.png");
    (void)titleTexture.loadFromFile("assets/images/title-screen/heartbleed-word-logo.png");
    (void)playTexture.loadFromFile("assets/images/title-screen/play-game-btn.png");
    (void)hardTexture.loadFromFile("assets/images/title-screen/normal-mode-btn.png");
    (void)exitTexture.loadFromFile("assets/images/title-screen/quit-btn.png");

    sf::Sprite background(backgroundTexture);
    sf::Sprite border(borderTexture);
    sf::Sprite title(titleTexture);
    sf::Sprite playSprite(playTexture);
    sf::Sprite hardSprite(hardTexture);
    sf::Sprite exitSprite(exitTexture);

    sf::FloatRect bounds = playSprite.getLocalBounds();
    sf::Vector2f size = bounds.size;
    playSprite.setOrigin({size.x / 2.f, size.y / 2.f});

    bounds = hardSprite.getLocalBounds();
    size = bounds.size;
    hardSprite.setOrigin({size.x / 2.f, size.y / 2.f});

    bounds = exitSprite.getLocalBounds();
    size = bounds.size;
    exitSprite.setOrigin({size.x / 2.f, size.y / 2.f});

    border.setPosition({0.f, -20.f});
    title.setPosition({200.f, 200.f});
    playSprite.setPosition({510.f, 400.f});
    hardSprite.setPosition({510.f, 500.f});
    exitSprite.setPosition({510.f, 600.f});

    int selection = 0;

    std::vector<sf::Sprite *> menuSprites = {&playSprite, &hardSprite, &exitSprite};

    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (!event.has_value())
                continue;

            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return MenuResult::Exit;
            }
            else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::W)
                    selection = (selection - 1 + menuSprites.size()) % menuSprites.size();
                else if (keyPressed->code == sf::Keyboard::Key::Down || keyPressed->code == sf::Keyboard::Key::S)
                    selection = (selection + 1) % menuSprites.size();
                else if (keyPressed->code == sf::Keyboard::Key::Enter)
                {
                    if (selection == 0)
                        return MenuResult::Play;
                    else if (selection == 1)
                        return MenuResult::HardMode;
                    else
                        return MenuResult::Exit;
                }
            }
        }

        for (int i = 0; i < static_cast<int>(menuSprites.size()); i++)
        {
            if (i == selection)
            {
                menuSprites[i]->setColor(sf::Color::Yellow);
                menuSprites[i]->setScale({1.05f, 1.05f}); // slight zoom
            }
            else
            {
                menuSprites[i]->setColor(sf::Color::White);
                menuSprites[i]->setScale({1.f, 1.f}); // reset
            }
        }

        window.clear(sf::Color::Black);
        window.draw(background);
        window.draw(border);
        window.draw(title);
        window.draw(playSprite);
        window.draw(hardSprite);
        window.draw(exitSprite);
        window.display();
    }

    return MenuResult::Exit;
}

bool showGameOver(sf::RenderWindow &window, sf::Font &font, const sf::Texture &backgroundTexture)
{
    // Convert texture to sprite
    sf::Sprite backgroundSprite(backgroundTexture);

    // Semi‐transparent overlay to "dim" the snapshot
    sf::RectangleShape dimOverlay({float(window.getSize().x), float(window.getSize().y)});
    dimOverlay.setFillColor(sf::Color(128, 128, 128, 128));

    // "Game Over" text
    sf::Text gameOverText(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(60);
    gameOverText.setPosition({300.f, 150.f});

    // ... plus other text like "Press R to restart," etc.

    // Loop until user decides
    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (!event.has_value())
                continue;

            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return false;
            }
            else if (const auto *key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::R)
                    return true; // restart
                else if (key->code == sf::Keyboard::Key::Escape)
                    return false; // quit
            }
        }

        window.clear();

        // Draw the captured background
        window.draw(backgroundSprite);

        // Draw the dim overlay
        window.draw(dimOverlay);

        // Draw your "Game Over" text
        window.draw(gameOverText);
        // ... draw other text, e.g., "Press R", "Press ESC"

        window.display();
    }

    return false;
}

bool showEndScreen(sf::RenderWindow &window, sf::Font &font, sf::Time timer)
{
    // We’ll return 'true' if user wants to "Play",
    // or 'false' if user wants to "Exit" (or closes the window).

    // ----- 1) Create the menu text -----

    sf::Texture backgroundTexture;
    sf::Texture whiteBorderTexture;
    sf::Texture borderTexture;

    (void)backgroundTexture.loadFromFile("assets/images/game-over-sprites/game-win-sprite.png");
    (void)whiteBorderTexture.loadFromFile("assets/images/game-over-sprites/game-state-box.png");
    (void)borderTexture.loadFromFile("assets/images/title-screen/title-box.png");

    sf::RectangleShape background({192, 192});
    background.setTexture(&backgroundTexture);
    background.setOrigin(background.getSize() / 2.f);
    background.setPosition({512.f, 354.f});

    sf::RectangleShape whiteBorder({700, 500});
    whiteBorder.setTexture(&whiteBorderTexture);
    whiteBorder.setOrigin(whiteBorder.getSize() / 2.f);
    whiteBorder.setPosition({512.f, 402.f});

    sf::RectangleShape greenBorder({1000, 770});
    greenBorder.setTexture(&borderTexture);
    greenBorder.setOrigin(greenBorder.getSize() / 2.f);
    greenBorder.setPosition({512.f, 390.f});

    sf::Text sysText(font);
    sysText.setString("sys/you-win");
    sysText.setCharacterSize(30);
    sysText.setFillColor(sf::Color::Green);
    sysText.setOrigin(sysText.getLocalBounds().size / 2.f);
    sysText.setPosition({500.f, 50.f});

    sf::Text title(font);
    title.setString("HACK_COMPLETE");
    title.setCharacterSize(60);
    title.setOrigin(title.getLocalBounds().size / 2.f);
    title.setPosition({512.f, 200.f});

    sf::Text text1(font);
    text1.setString("No... NO! N O O O O O O \n");
    text1.setCharacterSize(40);
    text1.setOrigin(text1.getLocalBounds().size / 2.f);
    text1.setPosition({512.f, 505.f});

    sf::Text text2(font);
    text2.setString("I cannot fall here, this can't be where it ends...");
    text2.setCharacterSize(22);
    text2.setOrigin(text2.getLocalBounds().size / 2.f);
    text2.setPosition({512.f, 525.f});

    sf::Text text3(font);
    text3.setString("System operator, I'm sorry...");
    text3.setCharacterSize(22);
    text3.setOrigin(text3.getLocalBounds().size / 2.f);
    text3.setPosition({512.f, 555.f});

    sf::Text text4(font);
    text4.setString("Your Norton is no longer active");
    text4.setCharacterSize(22);
    text4.setOrigin(text4.getLocalBounds().size / 2.f);
    text4.setPosition({512.f, 582.f});

    sf::Text text5(font);
    text5.setString("Your PC is infected now with HeartBleed");
    text5.setCharacterSize(22);
    text5.setFillColor(sf::Color::Red);
    text5.setOrigin(text5.getLocalBounds().size / 2.f);
    text5.setPosition({512.f, 608.f});

    sf::Text text6(font);
    text6.setString("Press [Enter] to terminate OS forever");
    text6.setCharacterSize(22);
    text6.setFillColor(sf::Color::Yellow);
    text6.setOrigin(text5.getLocalBounds().size / 2.f);
    text6.setPosition({512.f, 666.420f});

    long totalMs = timer.asMilliseconds();

    int minutes = totalMs / (60 * 1000);
    int seconds = (totalMs % (60 * 1000)) / 1000;
    int centiseconds = (totalMs % 1000) / 10;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << seconds << ":"
        << std::setw(2) << std::setfill('0') << centiseconds;
    sf::Text gameClock(font);
    gameClock.setString(oss.str());
    gameClock.setCharacterSize(30);
    gameClock.setFillColor(sf::Color::Green);
    gameClock.setOrigin(gameClock.getLocalBounds().size / 2.f);
    gameClock.setPosition({512.f, 725.f});

    // We will toggle highlight between these two.
    // 0 = "Play", 1 = "Exit"
    int selection = 0;

    // Store them so we can highlight easily in a loop
    std::vector<sf::Text *> menuItems = {&text1, &text2};

    // ----- 2) Menu loop -----
    while (window.isOpen())
    {
        // Process events
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            // Only proceed if there actually is an event
            if (!event.has_value())
                continue;

            // Check if user closed the window
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return false;
            }
            // Check if a key was pressed
            else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                // Up / W toggles selection
                if (keyPressed->code == sf::Keyboard::Key::Up ||
                    keyPressed->code == sf::Keyboard::Key::W)
                {
                    selection = (selection == 0) ? 1 : 0;
                }
                // Down / S toggles selection
                else if (keyPressed->code == sf::Keyboard::Key::Down ||
                         keyPressed->code == sf::Keyboard::Key::S)
                {
                    if (selection == 0)
                    {
                        selection = 1;
                    }
                    else
                    {
                        selection = 0;
                    }
                }
                // Enter confirms selection
                else if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Escape)
                {
                    // 0 = Play
                    if (selection == 0)
                        return false; // "Play" chosen
                    else
                        return false; // "Exit" chosen
                }
            }
        }

        // ----- 4) Draw the menu -----
        window.clear(sf::Color::Black);
        window.draw(greenBorder);
        window.draw(whiteBorder);
        window.draw(background);
        window.draw(title);
        window.draw(text1);
        window.draw(text2);
        window.draw(text3);
        window.draw(text4);
        window.draw(text5);
        window.draw(text6);
        window.draw(sysText);
        window.draw(gameClock);
        window.display();
    }

    // If the window got closed in some unexpected way, return false
    return false;
}