#include "Player.hpp"
#include "Bullet.hpp"
Player::Player() : collider(body), bullet({})
{
    // Initialize base values of player
    Vector2f size = {64, 64};
    health = 100;
    position = Vector2f(200, 200);
    velocity = Vector2f(0, 0);
    speed = 150.0f;
    jumpPower = 125.0f;
    isOnGround = false;

    // Initialize inputs
    left = false;
    right = false;
    space = false;
    canFire = true;
    toggleMode = false;

    // Initialize sprite
    if (!texture.loadFromFile("assets/images/test.png"))
    {
        std::cerr << "Error loading in player texture" << std::endl;
    }

    body.setSize(size);
    body.setOrigin(size / 2.0f);
    body.setPosition(position);
    body.setTexture(&texture);
}

void Player::handleInput(Keyboard::Scancode key, bool checkPressed, RenderWindow &window)
{
    if (checkPressed == true)
    {
        if (key == Keyboard::Scancode::A)
        {
            body.setScale({-1.f, 1.f}); // Flip right
            left = true;
        }
        if (key == Keyboard::Scancode::D)
        {
            body.setScale({1.f, 1.f}); // Flip left
            right = true;
        }
        if (key == Keyboard::Scancode::Space && isOnGround)
        {
            isOnGround = false;
            jump();
        }
        if (key == Keyboard::Scancode::Enter)
        {
            isFiring = true;
        }
        if (key == Keyboard::Scancode::T)
        {
            toggleMode = !toggleMode;
        }
    }
    if (checkPressed == false)
    {
        if (key == Keyboard::Scancode::A)
        {
            left = false;
        }
        if (key == Keyboard::Scancode::D)
        {
            right = false;
        }
        if (key == Keyboard::Scancode::Space)
        {
            space = false;
        }
        if (key == Keyboard::Scancode::Enter)
        {
            isFiring = false;
        }
    }
}

void Player::applyGravity(float deltaTime)
{
    velocity.y += 981.0f * deltaTime;
}

void Player::move(float deltaTime)
{
    velocity.x *= 0.5f;

    if (left)
    {
        velocity.x -= speed;
    }
    if (right)
    {
        velocity.x += speed;
    }

    body.move(velocity * deltaTime);
}

void Player::jump()
{
    velocity.y = -sqrt(2.0f * 981.0f * jumpPower);
}

void Player::fire()
{
    if (canFire)
    {
        canFire = false;
        timeLastFired = inGameClock.getElapsedTime().asSeconds();
        bullet.setSize(sf::Vector2f(50, 5));
        bullet.setPos(body.getPosition());
        bullet.setDirection(body.getScale().x);
    }
}

void Player::takeDamage(int amount)
{
}

void Player::onCollision(Vector2f direction)
{
    if (direction.x < 0.0f)
    {
        velocity.x = 0.0f; // collision on the left
    }
    else if (direction.x > 0.0f)
    {
        velocity.x = 0.0f; // collision on the right
    }

    if (direction.y < 0.0f)
    {
        velocity.y = 0.0f; // colliding below
        isOnGround = true;
    }
    else if (direction.y > 0.0f)
    {
        velocity.y = 0.0f; // colliding above
    }
}

void Player::update()
{
    dt = deltaClock.restart();
    applyGravity(dt.asSeconds());
    move(dt.asSeconds());
    if (isFiring)
    {
        fire(window);
    }
    if (!canFire && inGameClock.getElapsedTime().asSeconds() - timeLastFired >= 0.5f)
    {
        canFire = true;
    }
}

void Player::render(RenderWindow &window)
{
    window.draw(body);
}

void Player::renderBullet(RenderWindow &window)
{
    bullet.draw(window, dt);
}

void Player::setPosition(float x, float y) {
	body.setPosition(Vector2f(x,y)); 
}

Vector2f Player::getPosition() {
	return body.getPosition();
}
