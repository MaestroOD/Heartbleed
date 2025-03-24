#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Bullet.hpp"
#include "Collider.hpp"

using namespace sf;

class Enemy
{
public:
    Enemy(sf::Vector2f size, sf::Color color, bool canMove);
    void setPos(sf::Vector2f newPos);
    void setColor(sf::Color color);
    void checkBullet(Bullet bullet);
    void draw(sf::RenderWindow &window);
    void update(float deltaTime, sf::Vector2f playerPos);
    void setSpeed(float newSpeed);
    void setDetectionRange(float newRange);
    void onCollision(sf::Vector2f direction);

    Collider &getCollider() { return collider; }

private:
    sf::RectangleShape enemy;
    int health;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
    float range = 250.0f;
    Collider collider;
};