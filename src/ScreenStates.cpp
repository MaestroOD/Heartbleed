#include "ScreenState.hpp"
#include <optional>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

MenuResult showMenu(sf::RenderWindow &window, sf::Font &font)
{

    sf::Texture backgroundTexture;
    sf::Texture borderTexture; // I fucking hate this shit, sfml just doesn't like the name greenBorderTexture
    sf::Texture titleTexture;
    sf::Texture playTexture;
    sf::Texture hardTexture;
    sf::Texture exitTexture;
    sf::Texture wasdTexture;

    (void)backgroundTexture.loadFromFile("assets/images/title-screen/title-bg.png");
    (void)borderTexture.loadFromFile("assets/images/title-screen/title-box.png");
    (void)titleTexture.loadFromFile("assets/images/title-screen/heartbleed-word-logo.png");
    (void)playTexture.loadFromFile("assets/images/title-screen/play-game-btn.png");
    (void)hardTexture.loadFromFile("assets/images/title-screen/normal-mode-btn.png");
    (void)exitTexture.loadFromFile("assets/images/title-screen/quit-btn.png");
    (void)wasdTexture.loadFromFile("assets/images/title-screen/wasd.png");

    sf::Sprite background(backgroundTexture);
    sf::Sprite border(borderTexture);
    sf::Sprite title(titleTexture);
    sf::Sprite playSprite(playTexture);
    sf::Sprite hardSprite(hardTexture);
    sf::Sprite exitSprite(exitTexture);
    sf::Sprite wasdSprite(wasdTexture);

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
    wasdSprite.setPosition({350.f, 600.f});

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
                if (keyPressed->code == sf::Keyboard::Key::H) {
                    return MenuResult::HardMode;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::W)
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
        window.draw(wasdSprite);
        window.display();
    }
    return MenuResult::Exit;
}

MenuResult showPaused(sf::RenderWindow &window, sf::Font &font, const sf::Texture &backgroundTexture, sf::Time timer, sf::Music &music, Player &player)
{
    sf::Sprite backgroundSprite(backgroundTexture);
    sf::Texture whiteBorderTexture;
    sf::Texture resumeButtonTexture;

    (void)whiteBorderTexture.loadFromFile("assets/images/game-over-sprites/game-state-box.png");
    (void)resumeButtonTexture.loadFromFile("assets/images/title-screen/quit-box.png");

    // Dim overlay
    sf::RectangleShape dimOverlay({float(window.getSize().x), float(window.getSize().y)});
    dimOverlay.setFillColor(sf::Color(30, 30, 30, 200));

    sf::RectangleShape whiteBorder({450, 530});
    whiteBorder.setTexture(&whiteBorderTexture);
    whiteBorder.setFillColor(sf::Color::Green);
    whiteBorder.setOrigin(whiteBorder.getSize() / 2.f);
    whiteBorder.setPosition({500.f, 375.f});

    // Texts
    sf::Text sysText(font);
    sysText.setString("sys/paused");
    sysText.setCharacterSize(30);
    sysText.setFillColor(sf::Color::Green);
    sysText.setOrigin(sysText.getLocalBounds().size / 2.f);
    sysText.setPosition({500.f, 50.f});

    sf::RectangleShape resumeButton({350.f, 100.f});
    resumeButton.setTexture(&resumeButtonTexture);
    resumeButton.setFillColor(sf::Color::Green);
    resumeButton.setOrigin(resumeButton.getSize() / 2.f);
    resumeButton.setPosition({500.f, 225.f});

    sf::Text resumeButtonFont(font);
    resumeButtonFont.setString("Resume [ESC]");
    resumeButtonFont.setCharacterSize(40);
    resumeButtonFont.setFillColor(sf::Color::Magenta);
    resumeButtonFont.setOrigin(resumeButtonFont.getLocalBounds().size / 2.f);
    resumeButtonFont.setPosition({500.f, 220.f});

    sf::RectangleShape settingsButton({350.f, 100.f});
    settingsButton.setTexture(&resumeButtonTexture);
    settingsButton.setFillColor(sf::Color::Green);
    settingsButton.setOrigin(settingsButton.getSize() / 2.f);
    settingsButton.setPosition({500.f, 375.f});

    sf::Text settingsButtonFont(font);
    settingsButtonFont.setString("Settings");
    settingsButtonFont.setCharacterSize(40);
    settingsButtonFont.setFillColor(sf::Color::Red);
    settingsButtonFont.setOrigin(settingsButtonFont.getLocalBounds().size / 2.f);
    settingsButtonFont.setPosition({500.f, 365.f});

    sf::RectangleShape exitButton({350.f, 100.f});
    exitButton.setTexture(&resumeButtonTexture);
    exitButton.setFillColor(sf::Color::Green);
    exitButton.setOrigin(exitButton.getSize() / 2.f);
    exitButton.setPosition({500.f, 525.f});

    sf::Text exitButtonFont(font);
    exitButtonFont.setString("Exit");
    exitButtonFont.setCharacterSize(40);
    exitButtonFont.setFillColor(sf::Color::Cyan);
    exitButtonFont.setOrigin(exitButtonFont.getLocalBounds().size / 2.f);
    exitButtonFont.setPosition({500.f, 515.f});

    // Timer
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
    gameClock.setPosition({495.f, 725.f});

    // Selection logic
    int selectedIndex = 0;
    std::vector<sf::RectangleShape *> buttons = {&resumeButton, &settingsButton, &exitButton};
    std::vector<sf::Text *> texts = {&resumeButtonFont, &settingsButtonFont, &exitButtonFont};

    auto updateButtonScale = [&](int selected)
    {
        for (int i = 0; i < buttons.size(); ++i)
        {
            if (i == selected)
            {
                buttons[i]->setScale({1.1f, 1.1f});
                texts[i]->setScale({1.1f, 1.1f});
            }
            else
            {
                buttons[i]->setScale({1.f, 1.f});
                texts[i]->setScale({1.f, 1.f});
            }
        }
    };

    updateButtonScale(selectedIndex);

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
            else if (const auto *key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Escape)
                {
                    return MenuResult::Play;
                }
                else if (key->code == sf::Keyboard::Key::Up)
                {
                    selectedIndex = (selectedIndex - 1 + buttons.size()) % buttons.size();
                    updateButtonScale(selectedIndex);
                }
                else if (key->code == sf::Keyboard::Key::Down)
                {
                    selectedIndex = (selectedIndex + 1) % buttons.size();
                    updateButtonScale(selectedIndex);
                }
                else if (key->code == sf::Keyboard::Key::Enter)
                {
                    switch (selectedIndex)
                    {
                    case 0:
                        return MenuResult::Play;
                        break;
                    case 1:
                    {
                        MenuResult result = showSettings(window, font, music, player);
                        if (result == MenuResult::Exit)
                            return MenuResult::Exit;
                        // After settings, go back to paused menu
                        updateButtonScale(selectedIndex); // re-apply button selection
                        break;
                    }

                    case 2:
                        return MenuResult::Exit;
                        break;
                    }
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(dimOverlay);
        window.draw(sysText);
        window.draw(whiteBorder);
        window.draw(resumeButton);
        window.draw(settingsButton);
        window.draw(exitButton);
        window.draw(resumeButtonFont);
        window.draw(settingsButtonFont);
        window.draw(exitButtonFont);
        window.draw(gameClock);
        window.display();
    }

    return MenuResult::Exit;
}

MenuResult showSettings(sf::RenderWindow &window, sf::Font &font, sf::Music &music, Player &player)
{
    sf::Texture whiteBorderTexture;
    sf::Texture exitTexture;

    (void)whiteBorderTexture.loadFromFile("assets/images/game-over-sprites/game-state-box.png");
    (void)exitTexture.loadFromFile("assets/images/title-screen/quit-btn.png");

    // Background overlay
    sf::RectangleShape dimOverlay({float(window.getSize().x), float(window.getSize().y)});
    dimOverlay.setFillColor(sf::Color(50, 50, 50, 220));

    sf::RectangleShape whiteBorder({450, 530});
    whiteBorder.setTexture(&whiteBorderTexture);
    whiteBorder.setFillColor(sf::Color::Green);
    whiteBorder.setOrigin(whiteBorder.getSize() / 2.f);
    whiteBorder.setPosition({500.f, 375.f});

    sf::RectangleShape exitButton({250, 50});
    exitButton.setTexture(&exitTexture);
    exitButton.setFillColor(sf::Color::White);
    exitButton.setOrigin(exitButton.getSize() / 2.f);
    exitButton.setPosition({500.f, 585.f});

    sf::RectangleShape changeBox({160, 38});
    changeBox.setTexture(&whiteBorderTexture);
    changeBox.setPosition({300.f, 300.f});

    sf::Text changeKeysFont(font);
    changeKeysFont.setString("Change keys");
    changeKeysFont.setCharacterSize(20);
    changeKeysFont.setFillColor(sf::Color::White);
    changeKeysFont.setPosition({310.f, 305.f});

    sf::Text sysText(font);
    sysText.setString("sys/western/settings");
    sysText.setCharacterSize(30);
    sysText.setFillColor(sf::Color::Green);
    sysText.setOrigin(sysText.getLocalBounds().size / 2.f);
    sysText.setPosition({500.f, 50.f});

    // Title text
    sf::Text title(font);
    title.setString("Settings");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::White);
    title.setPosition({390.f, 125.f});

    sf::Text leftKeyFont(font);
    leftKeyFont.setString("Left Movement Key");
    leftKeyFont.setCharacterSize(20);
    leftKeyFont.setFillColor(sf::Color::White);
    leftKeyFont.setPosition({300.f, 345.f});

    sf::RectangleShape leftKeyButton({45, 45});
    leftKeyButton.setTexture(&whiteBorderTexture);
    leftKeyButton.setPosition({625.f, 330.f});
    leftKeyButton.scale({1.2, 1.2});

    sf::Text leftKeyButtonFont(font);
    leftKeyButtonFont.setString("A");
    leftKeyButtonFont.setCharacterSize(20);
    leftKeyButtonFont.setFillColor(sf::Color::White);
    leftKeyButtonFont.setPosition({647.f, 340.f});

    sf::Text rightKeyFont(font);
    rightKeyFont.setString("Right Movement Key");
    rightKeyFont.setCharacterSize(20);
    rightKeyFont.setPosition({300.f, 420.f});

    sf::RectangleShape rightKeyButton({45, 45});
    rightKeyButton.setTexture(&whiteBorderTexture);
    rightKeyButton.setPosition({625.f, 405.f});
    rightKeyButton.scale({1.2, 1.2});

    sf::Text rightKeyButtonFont(font);
    rightKeyButtonFont.setString("D");
    rightKeyButtonFont.setCharacterSize(20);
    rightKeyButtonFont.setPosition({647.f, 417.f});

    sf::Text shootKeyFont(font);
    shootKeyFont.setString("Shoot Key");
    shootKeyFont.setCharacterSize(20);
    shootKeyFont.setPosition({300.f, 495.f});

    sf::RectangleShape shootKeyButton({45, 45});
    shootKeyButton.setTexture(&whiteBorderTexture);
    shootKeyButton.setPosition({625.f, 485.f});
    shootKeyButton.scale({1.2, 1.2});

    sf::Text shootKeyFontButton(font);
    shootKeyFontButton.setString("return");
    shootKeyFontButton.setCharacterSize(13);
    shootKeyFontButton.setPosition({627.f, 500.f});

    sf::Text musicText(font);
    musicText.setString("Music [Use mouse to change volume]");
    musicText.setCharacterSize(20);
    musicText.setFillColor(sf::Color::Green);
    musicText.setOrigin(musicText.getLocalBounds().size / 2.f);
    musicText.setPosition({500.f, 215.f});

    // Slider bar (the track)
    sf::RectangleShape sliderTrack(sf::Vector2f(300.f, 10.f));
    sliderTrack.setFillColor(sf::Color::White);
    sliderTrack.setOrigin(sliderTrack.getSize() / 2.f);
    sliderTrack.setPosition({500.f, 270.f});

    // Slider knob (the handle)
    sf::CircleShape sliderKnob(15.f);
    sliderKnob.setFillColor(sf::Color::Green);
    sliderKnob.setOrigin({15.f, 15.f});
    float sliderStartX = sliderTrack.getPosition().x - sliderTrack.getSize().x / 2.f;
    float sliderEndX = sliderTrack.getPosition().x + sliderTrack.getSize().x / 2.f;
    float initialVolume = music.getVolume();
    sliderKnob.setPosition({sliderStartX + (initialVolume / 100.f) * (sliderEndX - sliderStartX), sliderTrack.getPosition().y});

    bool dragging = false;
    int selectedIndex = 0;

    std::vector<sf::RectangleShape *> buttons = {&changeBox, &exitButton};

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
            else if (const auto *key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Escape)
                {
                    return MenuResult::Paused;
                }
                else if (key->code == sf::Keyboard::Key::Up)
                {
                    selectedIndex = (selectedIndex - 1 + buttons.size()) % buttons.size();
                }
                else if (key->code == sf::Keyboard::Key::Down)
                {
                    selectedIndex = (selectedIndex + 1) % buttons.size();
                }
                else if (key->code == sf::Keyboard::Key::Enter)
                {
                    if (selectedIndex == 0)
                    {
                        bool isLeftHanded = player.changeControls();
                        if (isLeftHanded)
                        {
                            leftKeyButtonFont.setString("J");
                            rightKeyButtonFont.setString("L");
                            shootKeyFontButton.setString("Caps");
                            shootKeyFontButton.setPosition({635, 500.f});
                        }
                        else
                        {
                            leftKeyButtonFont.setString("A");
                            rightKeyButtonFont.setString("D");
                            shootKeyFontButton.setString("Return");
                            shootKeyFontButton.setPosition({627.f, 500.f});
                        }
                    }
                    else if (selectedIndex == 1)
                    {
                        return MenuResult::Paused;
                    }
                }
            }

            else if (const auto *mousePress = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mousePress->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (sliderKnob.getGlobalBounds().contains(mousePos))
                    {
                        dragging = true;
                    }
                }
            }
            else if (const auto *mouseRelease = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseRelease->button == sf::Mouse::Button::Left)
                {
                    dragging = false;
                }
            }
            else if (event->is<sf::Event::MouseMoved>() && dragging)
            {
                float mouseX = float(sf::Mouse::getPosition(window).x);
                mouseX = std::clamp(mouseX, sliderStartX, sliderEndX);
                sliderKnob.setPosition({mouseX, sliderTrack.getPosition().y});

                // Calculate volume percentage
                float volume = ((mouseX - sliderStartX) / (sliderEndX - sliderStartX)) * 100.f;

                // ✅ ACTUALLY SET THE MUSIC VOLUME
                music.setVolume(volume);
            }
        }

        for (int i = 0; i < buttons.size(); ++i)
        {
            if (i == selectedIndex)
                buttons[i]->setOutlineThickness(4.f), buttons[i]->setOutlineColor(sf::Color::Yellow);
            else
                buttons[i]->setOutlineThickness(0.f);
        }

        window.clear();
        window.draw(dimOverlay);
        window.draw(whiteBorder);
        window.draw(title);
        window.draw(musicText);
        window.draw(sysText);
        window.draw(changeBox);
        window.draw(changeKeysFont);
        window.draw(leftKeyFont);
        window.draw(leftKeyButton);
        window.draw(leftKeyButtonFont);
        window.draw(rightKeyFont);
        window.draw(rightKeyButton);
        window.draw(rightKeyButtonFont);
        window.draw(shootKeyFont);
        window.draw(shootKeyButton);
        window.draw(shootKeyFontButton);
        window.draw(sliderTrack);
        window.draw(sliderKnob);
        window.draw(exitButton);
        window.display();
    }

    return MenuResult::Exit;
}

bool showGameOver(sf::RenderWindow &window, sf::Font &font, sf::Time timer)
{
    std::vector<std::string> deathMessages;
    deathMessages.push_back("Still, I don't understand, were these chimps so human,\n              or are we such animals?");
    deathMessages.push_back("A strong man can hold in a fart, but only the wisest\n            can stop the wind from blowing.");
    deathMessages.push_back("Sometimes when I close my eyes,\n         I can't see.");
    deathMessages.push_back("It's not considered cheating\n  if you don't get caught.");

    srand(time(0));

    sf::Texture backgroundTexture;
    sf::Texture whiteBorderTexture;
    sf::Texture borderTexture;

    (void)backgroundTexture.loadFromFile("assets/images/game-over-sprites/game-lose-sprite.png");
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
    sysText.setString("sys/player_dead");
    sysText.setCharacterSize(30);
    sysText.setFillColor(sf::Color::Green);
    sysText.setOrigin(sysText.getLocalBounds().size / 2.f);
    sysText.setPosition({500.f, 50.f});

    sf::Text randomFact(font);
    int random = (std::rand() % (deathMessages.size()));
    randomFact.setString(deathMessages[random]);
    randomFact.setCharacterSize(20);
    randomFact.setOrigin(randomFact.getLocalBounds().size / 2.f);
    randomFact.setPosition({512.f, 470.f});
    std::cout << random;

    sf::Text title(font);
    title.setString("YOU_DIED");
    title.setCharacterSize(60);
    title.setOrigin(title.getLocalBounds().size / 2.f);
    title.setPosition({512.f, 200.f});

    sf::Text text1(font);
    text1.setString("Press [R] to attack the anti-virus again\n");
    text1.setCharacterSize(22);
    text1.setFillColor(sf::Color::Red);
    text1.setOrigin(text1.getLocalBounds().size / 2.f);
    text1.setPosition({512.f, 550.f});

    sf::Text text2(font);
    text2.setString("Press [ESC] to admit defeat \nand boot out of the system");
    text2.setCharacterSize(22);
    text2.setFillColor(sf::Color::Magenta);
    text2.setOrigin(text2.getLocalBounds().size / 2.f);
    text2.setPosition({512.f, 600.f});

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
                if (keyPressed->code == sf::Keyboard::Key::R)
                {
                    selection = 0;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Escape)
                {
                    selection = 1;
                }
                {
                    // 0 = Play
                    if (selection == 0)
                        return true; // "Play" chosen
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
        window.draw(randomFact);
        window.draw(text1);
        window.draw(text2);
        window.draw(sysText);
        window.draw(gameClock);
        window.display();
    }

    // If the window got closed in some unexpected way, return false
    return false;
}

bool showEndScreen(sf::RenderWindow &window, sf::Font &font, sf::Time timer)
{
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
    gameClock.setPosition({480.f, 725.f});

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