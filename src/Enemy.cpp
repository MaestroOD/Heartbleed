#include "Enemy.hpp"

Enemy::Enemy(sf::Vector2f size, sf::Color color, bool canMove)
{
    health = 100;
    velocity = Vector2f(0, 0);
    speed = 150.0f;
    enemy.setFillColor(color);
    enemy.setSize(size);
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

void Enemy::draw(sf::RenderWindow &window)
{
    window.draw(enemy);
}