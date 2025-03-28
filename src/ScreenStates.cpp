#include "ScreenState.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>

bool showMenu(sf::RenderWindow &window, sf::Font &font)
{
    // We’ll return 'true' if user wants to "Play",
    // or 'false' if user wants to "Exit" (or closes the window).

    // ----- 1) Create the menu text -----

    sf::Text title(font);
    title.setString("HeartBleed");
    title.setCharacterSize(60);
    title.setPosition({200.f, 100.f});

    sf::Text playText(font);
    playText.setString("Play Game");
    playText.setCharacterSize(40);
    playText.setPosition({300.f, 250.f});

    sf::Text hardModeText(font);
    hardModeText.setString("Hard Mode [H]");
    hardModeText.setCharacterSize(40);
    hardModeText.setPosition({300.f, 320.f});

    sf::Text exitText(font);
    exitText.setString("Exit Game");
    exitText.setCharacterSize(40);
    exitText.setPosition({300.f, 390.f});

    // We will toggle highlight between these two.
    // 0 = "Play", 1 = "HardMode", 2 = "Exit "
    int selection = 0;

    // Store them so we can highlight easily in a loop
    std::vector<sf::Text *> menuItems = {&playText, &hardModeText, &exitText};

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
                    if (selection == 0)
                    {
                        selection = 1;
                    }
                    else
                    {
                        selection = 0;
                    }
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
                else if (keyPressed->code == sf::Keyboard::Key::Enter)
                {
                    // 0 = Play
                    if (selection == 0)
                        return true; // "Play" chosen
                    else
                        return false; // "Exit" chosen
                }
            }
        }

        // ----- 3) Highlight selected item -----
        for (int i = 0; i < static_cast<int>(menuItems.size()); i++)
        {
            if (i == selection)
                menuItems[i]->setFillColor(sf::Color::Yellow); // highlight
            else
                menuItems[i]->setFillColor(sf::Color::White);
        }

        // ----- 4) Draw the menu -----
        window.clear(sf::Color(50, 50, 50));
        window.draw(title);
        window.draw(playText);
        window.draw(exitText);
        window.display();
    }

    // If the window got closed in some unexpected way, return false
    return false;
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

bool showEndScreen(sf::RenderWindow &window, sf::Font &font)
{
    // We’ll return 'true' if user wants to "Play",
    // or 'false' if user wants to "Exit" (or closes the window).

    // ----- 1) Create the menu text -----

    sf::Text title(font);
    title.setString("Thanks for playing the demo");
    title.setCharacterSize(60);
    title.setPosition({200.f, 100.f});

    sf::Text playText(font);
    playText.setString("Play Again?");
    playText.setCharacterSize(40);
    playText.setPosition({300.f, 250.f});

    sf::Text exitText(font);
    exitText.setString("Exit the Game");
    exitText.setCharacterSize(40);
    exitText.setPosition({300.f, 320.f});

    // We will toggle highlight between these two.
    // 0 = "Play", 1 = "Exit"
    int selection = 0;

    // Store them so we can highlight easily in a loop
    std::vector<sf::Text *> menuItems = {&playText, &exitText};

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
                else if (keyPressed->code == sf::Keyboard::Key::Enter)
                {
                    // 0 = Play
                    if (selection == 0)
                        return true; // "Play" chosen
                    else
                        return false; // "Exit" chosen
                }
            }
        }

        // ----- 3) Highlight selected item -----
        for (int i = 0; i < static_cast<int>(menuItems.size()); i++)
        {
            if (i == selection)
                menuItems[i]->setFillColor(sf::Color::Yellow); // highlight
            else
                menuItems[i]->setFillColor(sf::Color::White);
        }

        // ----- 4) Draw the menu -----
        window.clear(sf::Color(50, 50, 50));
        window.draw(title);
        window.draw(playText);
        window.draw(exitText);
        window.display();
    }

    // If the window got closed in some unexpected way, return false
    return false;
}