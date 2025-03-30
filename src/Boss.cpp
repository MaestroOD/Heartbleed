#include "Boss.hpp"

Boss::Boss() : Enemy({160, 160}, sf::Color::White, false), otherBullet({ 64, 64 }, 1)
{
	if (!texture.loadFromFile("assets/images/enemy/boss-idle-1.png")) 
	{
		std::cerr << "Cannot load in idle boss texture!";
	}
	if (!attackTexture.loadFromFile("assets/images/enemy/boss-idle-2.png")) 
	{
		std::cerr << "Cannot load in attack boss texture!";
	}
	if (!hurtTexture.loadFromFile("assets/images/enemy/boss-hurt.png"))
	{
		std::cerr << "Cannot load in hurt boss texture!";
	}

	enemy.setTexture(&texture);
	enemy.setTextureRect(sf::IntRect({ 0, 0 }, { 64, 64 }));
	health = 30;
	// Set both bullets to blue
	enemyBullet.setSpeed(1500.f);
	enemyBullet.setColor(sf::Color::Cyan);

	otherBullet.setPos({ 100000.f, 100000.f });
	otherBullet.setSpeed(1000.f);
	otherBullet.setDamage(1);
	otherBullet.setColor(sf::Color::Magenta);

	isBoss = true;
	std::srand(time(0));
}

void Boss::update(Time deltaTime) 
{
	dt = deltaTime;
	fireProjectile();
	fireOtherProjectile();

	if (enemyClock.getElapsedTime().asSeconds() - timeSinceAtk >= atkCooldown)
	{
		enableAttack();
	}
	else if (enemyClock.getElapsedTime().asSeconds() - timeSinceAtk >= atkCooldown / 2.f)
	{
		enemy.setTexture(&texture);
	}
	if (enemyClock.getElapsedTime().asSeconds() - timeSinceVAtk >= atkCooldown * 1.2f) {
		canVerticalAttack = true;
	}
	else if (enemyClock.getElapsedTime().asSeconds() - timeSinceVAtk >= atkCooldown / 2.f) {
		enemy.setTexture(&texture);
	}
}

void Boss::fireOtherProjectile() 
{
	if (health > 0 && canVerticalAttack)
	{
		enemy.setTexture(&attackTexture);
		canVerticalAttack = false;
		// fire projectile
		int dir = 1;
		timeSinceVAtk = enemyClock.getElapsedTime().asSeconds();
		otherBullet.setSize(sf::Vector2f(64, 64));
		otherBullet.setPos({ enemy.getPosition().x, 180.f + (std::rand() % 650) });
		if (enemy.getScale().x < 0)
		{
			dir = -1;
		}
		otherBullet.setDirection(dir);

		laserSound.play();
	}
}

void Boss::drawVBullet(RenderWindow &window) 
{
	otherBullet.draw(window, dt);
}

void Boss::takeDamage(int dmg)
{
	health -= dmg;
	if (health <= 0)
	{
		enemy.setPosition(sf::Vector2f(69696969, 6969696969));
	}
	std::cout << health;
	hurtSound.play();
	enemy.setTexture(&hurtTexture);
}