#pragma once
#include "Enemy.hpp"

class Boss : public Enemy {
private:
	Texture hurtTexture;
	Bullet otherBullet;
	float timeSinceVAtk;
	bool canVerticalAttack;

public:
	Boss();
	void update(Time deltaTime);
	void takeDamage(int dmg) override;
	void fireOtherProjectile();
	void drawVBullet(RenderWindow &window);

	Bullet &getOtherBullet() { return otherBullet; }; 

};