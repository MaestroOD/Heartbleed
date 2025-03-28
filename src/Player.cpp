#include "Player.hpp"
#include "Bullet.hpp"
#include <cmath>
#include <iostream>
Player::Player() : collider(sprite), bullet({}, 0), upBullet({}, 0), sprite(texture)
{
    // Initialize base values of player
    Vector2f size = {64, 64};
    health = 100;
    currentBullet = 1;
    position = Vector2f(200, 200);
    velocity = Vector2f(0, 0);
    speed = 150.0f;
    jumpPower = 125.0f;
    isOnGround = false;
    canJump = true;

    // Initialize inputs
    left = false;
    right = false;
    space = false;
    canFire = true;
    toggleMode = false;
    upgrade = true;

    bullet.setDamage(25);
    bullet.setSpeed(800.f);

    upBullet.setDamage(50);
    upBullet.setSpeed(1200.f);
    upBullet.setColor(sf::Color::Red);

    // Initialize sprite
    if (!texture.loadFromFile("assets/images/Player.png"))
    {
        std::cerr << "Error loading in player texture" << std::endl;
    }

    // Switched from Rect to Sprite
    rectSourceSprite = IntRect({0, 0}, {32, 32});
    sprite.setTexture(texture);
    sprite.setTextureRect(rectSourceSprite);
    sprite.setOrigin({16, 16});
    sprite.setScale({2.f, 2.f});
    sprite.setPosition(position);

}

void Player::handleInput(Keyboard::Scancode key, bool checkPressed, RenderWindow &window)
{
    if (checkPressed == true)
    {
        if (key == Keyboard::Scancode::A)
        {
            sprite.setScale({-2.f, 2.f}); // Flip right
            left = true;
        }
        if (key == Keyboard::Scancode::D)
        {
            sprite.setScale({2.f, 2.f}); // Flip left
            right = true;
        }
        if (key == Keyboard::Scancode::Space)
        {
            if (isOnGround || coyoteTime)
            {
                isOnGround = false;
                jump();
            }
        }
        if (key == Keyboard::Scancode::Enter)
        {
            isFiring = true;
        }
        if (key == Keyboard::Scancode::T)
        {
            toggleMode = !toggleMode;
        }
        if (upgrade && canFire)
        {
            if (key == Keyboard::Scancode::Num1)
            {
                std::cout << "Gun 1";
                currentBullet = 1;
            }
            else if (key == Keyboard::Scancode::Num2)
            {
                std::cout << "Gun 2";
                currentBullet = 2;
            }
        }
    }
    if (checkPressed == false)
    {
        if (key == Keyboard::Scancode::A)
        {
            left = false;
        }
        if (key == Keyboard::Scancode::D)
        {
            right = false;
        }
        if (key == Keyboard::Scancode::Space)
        {
            space = false;
        }
        if (key == Keyboard::Scancode::Enter)
        {
            isFiring = false;
        }
    }
}

void Player::applyGravity(float deltaTime)
{
    velocity.y += 981.0f * deltaTime;
}

void Player::move(float deltaTime)
{
    velocity.x *= 0.5f;

    if (left)
    {
        velocity.x -= speed;
    }
    if (right)
    {
        velocity.x += speed;
    }

    if (left || right)
    {
        // Falling
        if (!isOnGround)
        {
            rectSourceSprite.position.x = 32;
        }
        // Moving on ground
        else if (isOnGround && spriteClock.getElapsedTime().asSeconds() > 0.25f)
        {
            if (rectSourceSprite.position.x == 0 || rectSourceSprite.position.x == 96)
            {
                rectSourceSprite.position.x = 64;
            }
            else
            {
                rectSourceSprite.position.x = 96;
            }
            spriteClock.restart();
        }
    }
    // Idle
    else if (isOnGround)
    {
        rectSourceSprite.position.x = 0;
    }
    // Falling/jumping
    else
    {
        rectSourceSprite.position.x = 32;
    }

    sprite.move(velocity * deltaTime);
}

void Player::jump()
{
    if (canJump)
    {
        canJump = false;
        velocity.y = -sqrt(2.0f * 981.0f * jumpPower);
    }
}

void Player::fire()
{
    if (canFire)
    {
        int dir = 1;
        canFire = false;
        timeLastFired = inGameClock.getElapsedTime().asSeconds();
        if (sprite.getScale().x < 0)
        {
            dir = -1;
        }

        if (currentBullet == 1)
        {
            bullet.setSize(sf::Vector2f(32, 32));
            bullet.setPos(sprite.getPosition());
            bullet.setDirection(dir);
        }
        else if (currentBullet == 2)
        {
            upBullet.setSize(sf::Vector2f(32, 32));
            upBullet.setPos(sprite.getPosition());
            upBullet.setDirection(dir);
        }
    }
}

void Player::takeDamage(int amount)
{
    if (health >= amount)
    {
        health -= amount;
    }
    else
    {
        health = 0;
    }
}

void Player::onCollision(Vector2f direction)
{
    if (direction.x < 0.0f)
    {
        velocity.x = 0.0f; // collision on the left
    }
    else if (direction.x > 0.0f)
    {
        velocity.x = 0.0f; // collision on the right
    }

    if (direction.y < 0.0f)
    {
        velocity.y = 0.0f; // colliding below
        isOnGround = true;
        canJump = true;
        coyoteTime = false;
    }
    else if (direction.y > 0.0f)
    {
        velocity.y = 0.0f; // colliding above
    }
}

void Player::checkEnemyBullet(Bullet &bullet, int damage)
{
    sf::Vector2f otherPosition = bullet.getPos();
    sf::Vector2f otherHalfSize = bullet.getHalfSize();
    sf::Vector2f thisPosition = sprite.getPosition();
    sf::Vector2f thisHalfSize = {16, 16};

    float deltaX = otherPosition.x - thisPosition.x;
    float deltaY = otherPosition.y - thisPosition.y;
    float intersectX = std::abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
    float intersectY = std::abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);

    if (intersectX < 0.f && intersectY < 0.f)
    {
        bullet.setPos(sf::Vector2f(420420420, 420420420));
        takeDamage(damage);
    }
}

void Player::update()
{
    dt = deltaClock.restart();
    applyGravity(dt.asSeconds());
    move(dt.asSeconds());
    if (isFiring)
    {
        fire();
    }
    if (!canFire && inGameClock.getElapsedTime().asSeconds() - timeLastFired >= 0.5f)
    {
        canFire = true;
    }
    // Check if free falling
    if (velocity.y > 0.0f)
    {
        // Initiate coyote time
        if (isOnGround)
        {
            isOnGround = false;
            timeSinceFall = inGameClock.getElapsedTime().asSeconds();
        }
        if (!coyoteTime && canJump && inGameClock.getElapsedTime().asSeconds() - timeSinceFall < 0.15f)
        {
            coyoteTime = true;
        }
    }
    // Check if coyote time is over
    if (coyoteTime && inGameClock.getElapsedTime().asSeconds() - timeSinceFall >= 0.15f)
    {
        coyoteTime = false;
    }
    // Set sprite animation
    sprite.setTextureRect(rectSourceSprite);
}

void Player::render(RenderWindow &window)
{
    window.draw(sprite);
}

void Player::renderBullet(RenderWindow &window)
{
    if (currentBullet == 1)
    {
        bullet.draw(window, dt);
    }
    else if (currentBullet == 2)
    {
        upBullet.draw(window, dt);
    }
}

Bullet *Player::getBullet()
{
    if (currentBullet == 2)
    {
        return &upBullet;
    }
    return &bullet;
}

void Player::setPosition(Vector2f pos)
{
    sprite.setPosition(pos);
}

Vector2f Player::getPosition()
{
    return sprite.getPosition();
}

void Player::setHP(int hp)
{
    health = hp;
}