#include "Enemy.hpp"
#include <cmath>

Enemy::Enemy(sf::Vector2f size, sf::Color color, bool canMove) : collider(enemy)
{
    health = 100;
    velocity = Vector2f(0, 0);
    speed = 150.0f;
    enemy.setFillColor(color);
    enemy.setSize(size);
    enemy.setOrigin(size / 2.0f);
}

void Enemy::setPos(sf::Vector2f newPos)
{
    enemy.setPosition(newPos);
}

void Enemy::setColor(sf::Color color)
{
    enemy.setFillColor(color);
}

void Enemy::checkBullet(Bullet bullet)
{
    if (bullet.getRight() > enemy.getPosition().x && bullet.getTop() < enemy.getPosition().y + enemy.getPosition().y && bullet.getBottom() > enemy.getPosition().y)
    {
        enemy.setPosition(sf::Vector2f(69696969, 6969696969));
    }
}

void Enemy::update(float deltaTime, Vector2f playerPos)
{
    // Apply gravity
    velocity.y += 981.0f * deltaTime;

    // Only move toward player on x-axis if in range
    sf::Vector2f direction = playerPos - enemy.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < range)
    {
        // Normalize X direction only (keep Y from gravity)
        float dx = direction.x;
        float dy = direction.y;
        float length = std::sqrt(dx * dx + dy * dy);
        if (length != 0)
        {
            direction /= length;
            velocity.x = direction.x * speed; // ← move left/right only
        }
    }
    else
    {
        velocity.x = 0; // no horizontal movement
    }

    enemy.move(velocity * deltaTime);
}

void Enemy::setSpeed(float newSpeed)
{
    speed = newSpeed;
}

void Enemy::setDetectionRange(float newRange)
{
    range = newRange;
}

void Enemy::onCollision(Vector2f direction)
{
    if (direction.y < 0.0f)
    {
        velocity.y = 0.0f; // landed on ground
    }
    if (direction.y > 0.0f)
    {
        velocity.y = 0.0f; // hit ceiling
    }
    if (direction.x != 0.0f)
    {
        velocity.x = 0.0f; // hit side wall
    }
}

void Enemy::draw(sf::RenderWindow &window)
{
    window.draw(enemy);
}