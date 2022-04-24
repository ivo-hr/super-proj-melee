#pragma once

#include "Entity.h"


class Projectile : public Entity
{
protected:
	int damage;
	float range;
	int lag = 6;
	Vector2D* iniPos;
	b2Vec2 vecDir;
	attackData data;
	float ang;

public:
	Projectile(FightManager* manager, Vector2D* pos, b2Vec2 dir, float width, float height, int speed);
	~Projectile();

	virtual void update() override;
	virtual void draw() override;
	virtual void draw(SDL_Rect* camera) override;
	virtual void CheckHits() override;
	virtual bool GetHit(attackData a, Entity* attacker) { return(false);};
	virtual SDL_Rect* GetHurtbox() { return &hurtbox;};
	virtual void changeDir() override;

};