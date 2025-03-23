#include "Bullet.hpp"

Bullet::Bullet(sf::Vector2f size)
{
    bullet.setSize(size);
    bullet.setFillColor(sf::Color::Black);
}

void Bullet::fire(sf::Vector2f speed, Time dt)
{
    bullet.move(speed * direction * dt.asSeconds());
}

void Bullet::draw(sf::RenderWindow &window, Time dt)
{
    fire({1000.0f, 0.0f}, dt);
    window.draw(bullet);
}

void Bullet::setPos(sf::Vector2f newPos)
{
    bullet.setPosition(newPos);
}

void Bullet::setSize(Vector2f size)
{
    bullet.setSize(size);
}

void Bullet::setDirection(float dir)
{
    direction = dir;
}