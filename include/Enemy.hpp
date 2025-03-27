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
    void checkBullet(Bullet& bullet);
    void draw(sf::RenderWindow& window);
    void drawBullet(sf::RenderWindow& window);
    void update(Time deltaTime, sf::Vector2f playerPos);
    void setSpeed(float newSpeed);
    void setDetectionRange(float newRange);
    void setDamage(int dmg);
    void setDirection(float dir); // For turret only
    void takeDamage(int amount);
    void disableAttack(); // Will be called in main
    void enableAttack();  // Will be called ONLY in Enemy.cpp
    void setAtkCooldown(float cooldown);
    bool getCanAttack() { return canAttack; };
    void fireProjectile();
    int getDamage() { return damage; };
    void onCollision(sf::Vector2f direction);
    void setMove(bool move);

    Bullet& getBullet() { return enemyBullet; };
    Collider& getCollider() { return collider; }

private:
    sf::Texture texture; // Idle texture
    sf::Texture attackTexture; // Attack texture
    sf::RectangleShape enemy; // Change to a sprite later
    int health;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
    float range = 250.0f;
    bool canMove;
    Collider collider;

    int damage;
    bool canAttack;
    float atkCooldown;
    float timeSinceAtk;
    Time dt;
    Clock enemyClock;
    Bullet enemyBullet;
};