#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Bullet.hpp"
#include "Collider.hpp"

using namespace sf;

class Enemy
{
public:
    Enemy(sf::Vector2f size, sf::Color color, bool canMove);
    Enemy(const Enemy& other);
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
    virtual void takeDamage(int amount);
    void disableAttack(); // Will be called in main
    void enableAttack();  // Will be called ONLY in Enemy.cpp
    void setAtkCooldown(float cooldown);
    bool getCanAttack() { return canAttack; };
    bool getCanMove() { return canMove; };
    void fireProjectile();
    int getDamage() { return damage; };
    void onCollision(sf::Vector2f direction);
    void setMove(bool move);
    void setAsWall();
    void printStatus() const;

    Bullet& getBullet() { return enemyBullet; };
    Collider& getCollider() { return collider; };
    sf::SoundBuffer& getSoundBuffer(std::string soundName);

protected:
    sf::Texture texture;       // Idle texture
    sf::Texture attackTexture; // Attack texture
    sf::RectangleShape enemy;  // Change to a sprite later
    int health;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
    float range = 250.0f;
    bool canMove;
    bool isWall;
    bool isBoss;
    sf::SoundBuffer hurtBuffer;
    sf::Sound hurtSound;
    sf::SoundBuffer laserBuffer;
    sf::Sound laserSound;
    sf::SoundBuffer screamBuffer;
    sf::Sound screamSound;
    Collider collider;

    int damage;
    bool canAttack;
    float atkCooldown;
    float timeSinceAtk;
    float timeSinceScream; // hehehehaw
    Time dt;
    Clock enemyClock;
    Bullet enemyBullet;
};