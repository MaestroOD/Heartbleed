#include "Enemy.hpp"
#include <iostream>
#include <cmath>

Enemy::Enemy(sf::Vector2f size, sf::Color color, bool cMove) : collider(enemy), enemyBullet({32, 32}, 1)
{
    health = 100;
    velocity = Vector2f(0, 0);
    speed = 150.0f;
    setDamage(10);
    canAttack = true;
    atkCooldown = 0.8f;
    canMove = cMove;

    enemy.setFillColor(color);
    enemy.setSize(size);
    enemy.setOrigin(size / 2.0f);
}

void Enemy::setDamage(int dmg)
{
    damage = dmg;
}

void Enemy::disableAttack()
{
    timeSinceAtk = enemyClock.getElapsedTime().asSeconds();
    canAttack = false;
}

void Enemy::enableAttack()
{
    canAttack = true;
}

void Enemy::setPos(sf::Vector2f newPos)
{
    enemy.setPosition(newPos);
}

void Enemy::setColor(sf::Color color)
{
    enemy.setFillColor(color);
}

void Enemy::checkBullet(Bullet &bullet)
{
    sf::Vector2f otherPosition = bullet.getPos();
    sf::Vector2f otherHalfSize = bullet.getHalfSize();
    sf::Vector2f thisPosition = enemy.getPosition();
    sf::Vector2f thisHalfSize = enemy.getSize() / 2.0f;

    float deltaX = otherPosition.x - thisPosition.x;
    float deltaY = otherPosition.y - thisPosition.y;
    float intersectX = std::abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
    float intersectY = std::abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);

    if (intersectX < 0.f && intersectY < 0.f)
    {
        bullet.setPos(sf::Vector2f(420420420, 420420420));
        takeDamage(50);
    }
}

void Enemy::takeDamage(int dmg)
{
    health -= dmg;
    if (health <= 0)
    {
        enemy.setPosition(sf::Vector2f(69696969, 6969696969));
    }
}

void Enemy::update(Time deltaTime, Vector2f playerPos)
{
    dt = deltaTime;
    // Apply gravity
    velocity.y += 981.0f * deltaTime.asSeconds();

    // Only move toward player on x-axis if in range
    sf::Vector2f direction = playerPos - enemy.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < range && canMove)
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
        if (!canMove)
        {
            // Fire bulelt periodically
            fireProjectile();
        }
    }

    if (enemyClock.getElapsedTime().asSeconds() - timeSinceAtk >= atkCooldown)
    {
        enableAttack();
    }

    enemy.move(velocity * deltaTime.asSeconds());
}

void Enemy::fireProjectile()
{
    if (canAttack)
    {
        disableAttack();
        timeSinceAtk = enemyClock.getElapsedTime().asSeconds();
        // fire projectile
        int dir = 1;
        enemyBullet.setSize(sf::Vector2f(32, 32));
        enemyBullet.setPos(enemy.getPosition());
        if (enemy.getScale().x < 0)
        {
            dir = -1;
        }
        enemyBullet.setDirection(dir);
    }
}

void Enemy::setDirection(float dir)
{
    enemy.setScale({dir, 1.0f});
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

void Enemy::drawBullet(sf::RenderWindow &window)
{
    enemyBullet.draw(window, dt);
}