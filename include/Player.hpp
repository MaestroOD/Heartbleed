#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.hpp"
#include "Bullet.hpp"
#include <iostream>

using namespace sf;

class Player
{
private:
    Vector2f position;
    Vector2f direction;
    Vector2f velocity;
    float speed;
    float jumpPower;
    float timeLastFired;
    float timeSinceFall;
    int health; // 100 Default
    bool isOnGround;
    bool coyoteTime;
    bool canJump;
    Texture texture;
    RectangleShape body;
    IntRect rectSourceSprite;
    Sprite sprite;
    Collider collider;
    Bullet bullet;

    // Input bools
    bool left;
    bool right;
    bool space;
    bool isFiring;
    bool canFire;
    bool toggleMode;

    Time dt; // DeltaTime
    Clock deltaClock;
    Clock inGameClock; // Total elapsed time in-game
    Clock spriteClock;

public:
    Player();
    // Is run every frame, checks for user input and fires function corresponding to input
    void handleInput(Keyboard::Scancode key, bool checkPressed, RenderWindow &window);
    // Gravity sim, run every physics update
    void applyGravity(float detlaTime);
    void move(float deltaTime);
    // Fixed force
    void jump();
    void fire();
    int getHP() { return health; };
    void takeDamage(int amount);
    void update();
    void render(RenderWindow &window);
    void renderBullet(RenderWindow &window);
    Bullet *getBullet() { return &bullet; };
    void checkEnemyBullet(Bullet &bullet, int damage);
    void setPosition(float x, float y);
    Vector2f getPosition();

    Collider &getCollider() { return collider; }
    void onCollision(Vector2f direction);

    bool getMode() { return toggleMode; };
};
