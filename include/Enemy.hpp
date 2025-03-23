#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Bullet.hpp"
using namespace sf;

class Enemy
{
public:
    Enemy(sf::Vector2f size, sf::Color color, bool canMove);
    void setPos(sf::Vector2f newPos);
    void setColor(sf::Color color);
    void checkBullet(Bullet bullet);
    void draw(sf::RenderWindow &window);

private:
    sf::RectangleShape enemy;
    int health;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
};