#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

class Bullet
{
public:
    Bullet(sf::Vector2f size, int owner);

    void fire(sf::Vector2f speed, Time dt);
    void setSize(sf::Vector2f size);
    int getOwner() { return owner; };
    Vector2f getPos() { return bullet.getPosition(); };
    Vector2f getHalfSize() { return bullet.getSize() / 2.0f; };
    void draw(sf::RenderWindow &window, Time dt);
    void setPos(sf::Vector2f newPos);
    void setDirection(float dir);
    int getDamage() { return damage; };
    void setDamage(int dmg);
    void setSpeed(float speed);
    void setColor(sf::Color col);
    void setTexture(const sf::Texture& newTex);

private:
    sf::RectangleShape bullet;
    sf::Texture texture;
    float direction;
    int owner; // 0 = Player, 1 = enemy
    int damage;
    float speed;
};