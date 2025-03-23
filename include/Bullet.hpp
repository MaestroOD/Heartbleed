#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

class Bullet
{
public:
    Bullet(sf::Vector2f size);

    void fire(sf::Vector2f speed, Time dt);
    void setSize(sf::Vector2f size);
    int getRight() { return bullet.getPosition().x + bullet.getSize().x; };
    int getLeft() { return bullet.getPosition().x; };
    int getTop() { return bullet.getPosition().y; };
    int getBottom() { return bullet.getPosition().y + bullet.getSize().y; }
    void draw(sf::RenderWindow &window, Time dt);
    void setPos(sf::Vector2f newPos);
    void setDirection(float dir);

private:
    sf::RectangleShape bullet;
    float direction;
};