#include "Bullet.hpp"

Bullet::Bullet(sf::Vector2f size, int o)
{
    owner = o;
    bullet.setSize(size);

    if (!texture.loadFromFile("assets/images/bullet.png"))
    {
        std::cerr << "Error loading in bullet texture" << std::endl;
    }

    bullet.setTexture(&texture);
}

void Bullet::fire(sf::Vector2f speed, Time dt)
{
    bullet.move(speed * direction * dt.asSeconds());
}

void Bullet::draw(sf::RenderWindow &window, Time dt)
{
    fire({speed, 0.0f}, dt);
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

void Bullet::setDamage(int dmg)
{
    damage = dmg;
}

void Bullet::setSpeed(float spd)
{
    speed = spd;
}

void Bullet::setColor(sf::Color col)
{
    bullet.setFillColor(col);
}