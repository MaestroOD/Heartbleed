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
	bool isFlashing = false;
	float flashDuration = 0.6f;   // how many seconds to stay in the hurt texture
	float flashTimer = 0.0f;

	Bullet &getOtherBullet() { return otherBullet; }; 

};