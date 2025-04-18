#include "Enemy.hpp"
#include <iostream>
#include <cmath>

Enemy::Enemy(sf::Vector2f size, sf::Color color, bool cMove) : collider(enemy), enemyBullet({32, 32}, 1), hurtSound(getSoundBuffer("hurt")), laserSound(getSoundBuffer("laser")), screamSound(getSoundBuffer("hehehehaw"))
{
    isWall = false;
    health = 4;
    velocity = Vector2f(0, 0);
    speed = 150.0f;
    setDamage(1);
    enemyBullet.setSpeed(700.f);
    enemyBullet.setDamage(1);
    enemyBullet.setPos({100000.f, 100000.f});
    canAttack = true;
    atkCooldown = 2.0f;
    canMove = cMove;
    timeSinceAtk = 1;

    direction = 1;

    if (canMove)
    {
        if (!texture.loadFromFile("assets/images/enemy/chaser.png"))
        {
            std::cerr << "Error: Unable to load in enemy sprite!";
        }
        if (!attackTexture.loadFromFile("assets/images/enemy/chaser-attack.png"))
        {
            std::cerr << "Error: Unable to load in enemy sprite!";
        }
    }
    else
    {
        if (!texture.loadFromFile("assets/images/enemy/turret.png"))
        {
            std::cerr << "Error: Unable to load in enemy sprite!";
        }
        if (!attackTexture.loadFromFile("assets/images/enemy/turret-attack.png"))
        {
            std::cerr << "Error: Unable to load in enemy sprite!";
        }
    }

    // Customize sound effects
    hurtSound.setVolume(10);
    hurtSound.setPitch(2);

    laserSound.setVolume(2);
    laserSound.setPitch(0.8);

    screamSound.setVolume(10);
    timeSinceScream = enemyClock.getElapsedTime().asSeconds();

    enemy.setTexture(&texture);
    enemy.setFillColor(color);
    enemy.setSize(size);
    enemy.setOrigin(size / 2.0f);

    std::srand(time(0));
}

Enemy::Enemy(const Enemy &other)
    : enemy(other.enemy),
      collider(enemy),
      enemyBullet(other.enemyBullet),
      health(other.health),
      velocity(other.velocity),
      speed(other.speed),
      damage(other.damage),
      canAttack(other.canAttack),
      atkCooldown(other.atkCooldown),
      canMove(other.canMove),
      attackTexture(other.attackTexture),
      texture(other.texture),
      hurtSound(getSoundBuffer("hurt")),
      laserSound(getSoundBuffer("laser")),
      screamSound(getSoundBuffer("hehehehaw")),
      isWall(other.isWall),
      isBoss(other.isBoss),
      direction(other.direction)
{
    // 1) Copy over your float-based times
    timeSinceScream = other.timeSinceScream;
    timeSinceAtk = other.timeSinceAtk;

    // 2) The new sf::Clock starts fresh:
    enemyClock.restart();

    // (Optional) If you want to know how much time had elapsed for the old enemy:
    float oldElapsed = other.enemyClock.getElapsedTime().asSeconds();
    // There's no built-in method in SFML to "set" the new clock to oldElapsed,
    // so you simply store that if needed, or ignore it.

    // If you still need the bullet texture, load or assign it here
    // (Better yet, pass by reference from some resource manager)
    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("assets/images/enemy/bullet-1-8.png"))
    {
        std::cerr << "Failed to load bullet textures\n";
    }

    enemy.setTexture(&texture);
}

void Enemy::setDamage(int dmg)
{
    damage = dmg;
}

void Enemy::disableAttack()
{
    timeSinceAtk = enemyClock.getElapsedTime().asSeconds();
    canAttack = false;
    enemy.setTexture(&attackTexture);
    speed = 0.0f;
}

void Enemy::enableAttack()
{
    canAttack = true;
    speed = 150.0f;
}

void Enemy::setPos(sf::Vector2f newPos)
{
    enemy.setPosition(newPos);
}

void Enemy::setColor(sf::Color color)
{
    enemy.setFillColor(color);
}

void Enemy::checkBullet(Bullet &bullet)
{
    sf::Vector2f otherPosition = bullet.getPos();
    sf::Vector2f otherHalfSize = bullet.getHalfSize();
    sf::Vector2f thisPosition = enemy.getPosition();
    sf::Vector2f thisHalfSize = enemy.getSize() / 2.0f;

    float deltaX = otherPosition.x - thisPosition.x;
    float deltaY = otherPosition.y - thisPosition.y;
    float intersectX = std::abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
    float intersectY = std::abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);

    if (intersectX < 0.f && intersectY < 0.f)
    {
        bullet.setPos(sf::Vector2f(420420420, 420420420));
        if (!isWall)
        {
            takeDamage(bullet.getDamage());
        }
        else if (bullet.getDamage() > 1)
        {
            takeDamage(bullet.getDamage());
        }
    }
}

void Enemy::takeDamage(int dmg)
{
    health -= dmg;
    if (health <= 0)
    {
        enemy.setPosition(sf::Vector2f(69696969, 6969696969));
    }
    std::cout << health;
    hurtSound.play();
}

void Enemy::update(Time deltaTime, Vector2f playerPos)
{
    if (isWall || health <= 0)
    {
        if (enemyClock.getElapsedTime().asSeconds() - timeSinceScream >= 5 && health > 0)
        {
            timeSinceScream = enemyClock.getElapsedTime().asSeconds();
            screamSound.play();
        }
        return;
    }
    dt = deltaTime;
    // Apply gravity
    velocity.y += 981.0f * deltaTime.asSeconds();

    // Only move toward player on x-axis if in range
    sf::Vector2f direction = playerPos - enemy.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < range && canMove)
    {
        // Normalize X direction only (keep Y from gravity)
        float dx = direction.x;
        float dy = direction.y;
        float length = std::sqrt(dx * dx + dy * dy);
        if (length != 0)
        {
            direction /= length;
            velocity.x = direction.x * speed; // ← move left/right only
        }
    }
    else
    {
        velocity.x = 0; // no horizontal movement
        if (!canMove)
        {
            // Fire bulelt periodically
            fireProjectile();
        }
    }

    if (enemyClock.getElapsedTime().asSeconds() - timeSinceAtk >= atkCooldown)
    {
        enableAttack();
    }
    else if (enemyClock.getElapsedTime().asSeconds() - timeSinceAtk >= atkCooldown / 2.f)
    {
        enemy.setTexture(&texture);
    }

    enemy.move(velocity * deltaTime.asSeconds());
}

void Enemy::fireProjectile()
{
    if (canAttack && !isWall && health > 0)
    {
        disableAttack();
        timeSinceAtk = (int)enemyClock.getElapsedTime().asSeconds();
        // fire projectile
        int dir = 1;
        enemyBullet.setSize(sf::Vector2f(32, 32));
        if (!isBoss)
        {
            enemyBullet.setPos(enemy.getPosition());
        }
        else
        {
            enemyBullet.setPos({enemy.getPosition().x, 180.f + (std::rand() % 650)});
        }

        if (enemy.getScale().x < 0)
        {
            dir = -1;
        }
        enemyBullet.setDirection(dir);

        laserSound.play();
    }
}

void Enemy::setDirection(float dir)
{
    enemy.setScale({dir, 1.0f});
}

void Enemy::setSpeed(float newSpeed)
{
    speed = newSpeed;
}

void Enemy::setDetectionRange(float newRange)
{
    range = newRange;
}

void Enemy::setAsWall()
{
    isWall = true;
    if (!texture.loadFromFile("assets/images/terrain/tile-corner.png"))
    {
        std::cerr << "Error: Unable to load in enemy sprite!";
    }

    enemy.setTexture(&texture);
    setColor(sf::Color::Red);
    hurtSound.setPitch(0.8);
}

void Enemy::setNotWall()
{
    isWall = false;
}

void Enemy::onCollision(Vector2f direction)
{
    if (direction.y < 0.0f)
    {
        velocity.y = 0.0f; // landed on ground
    }
    if (direction.y > 0.0f)
    {
        velocity.y = 0.0f; // hit ceiling
    }
    if (direction.x != 0.0f)
    {
        velocity.x = 0.0f; // hit side wall
    }
}

void Enemy::draw(sf::RenderWindow &window)
{
    window.draw(enemy);
}

void Enemy::drawBullet(sf::RenderWindow &window)
{
    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("assets/images/enemy/bullet-1-8.png"))
    {
        std::cerr << "Failed to load bullet textures\n";
    }
    enemyBullet.setTexture(bulletTexture);
    enemyBullet.draw(window, dt);
}

void Enemy::setMove(bool move)
{
    canMove = move;
}

SoundBuffer &Enemy::getSoundBuffer(std::string soundName)
{
    if (soundName.compare("hurt") == 0)
    {
        if (!hurtBuffer.loadFromFile("assets/audio/classic_hurt.wav"))
        {
            std::cerr << "Error loading hurt sound\n";
        }

        return hurtBuffer;
    }
    else if (soundName.compare("laser") == 0)
    {
        if (!laserBuffer.loadFromFile("assets/audio/laser.wav"))
        {
            std::cerr << "Error loading hurt sound\n";
        }

        return laserBuffer;
    }
    else
    {
        if (!screamBuffer.loadFromFile("assets/audio/heheha.wav"))
        {
            std::cerr << "Error loading hurt sound\n";
        }

        return screamBuffer;
    }
}

void Enemy::printStatus() const
{
    std::cout << "----- Enemy Status -----" << std::endl;
    std::cout << "Position: (" << enemy.getPosition().x << ", " << enemy.getPosition().y << ")" << std::endl;
    std::cout << "Velocity: (" << velocity.x << ", " << velocity.y << ")" << std::endl;
    std::cout << "Size: (" << enemy.getSize().x << ", " << enemy.getSize().y << ")" << std::endl;
    std::cout << "Health: " << health << std::endl;
    std::cout << "Speed: " << speed << std::endl;
    std::cout << "Detection Range: " << range << std::endl;
    std::cout << "Can Move: " << (canMove ? "Yes" : "No") << std::endl;
    std::cout << "Can Attack: " << (canAttack ? "Yes" : "No") << std::endl;
    std::cout << "Attack Cooldown: " << atkCooldown << std::endl;
    std::cout << "Time Since Last Attack: " << timeSinceAtk << std::endl;
    std::cout << "Damage: " << damage << std::endl;
    std::cout << "IsWall: " << isWall << std::endl;
    std::cout << "IsBoss: " << isBoss << std::endl;
    // std::cout << "Bullet Position: ("
    //     << enemyBullet.getPos().x << ", "
    //     << enemyBullet.getPos().y << ")" << std::endl;
    std::cout << "------------------------" << std::endl;
}