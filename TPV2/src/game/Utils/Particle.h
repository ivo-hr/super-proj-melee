#pragma once

#include <unordered_map>
#include <functional>
#include "../PlayingState/FightManager.h"
#include "../Entity.h"

class Entity;
class Fightmanager;

class Particle
{

protected:

	FightManager* manager;
	Entity* ent;

	Texture* texture;

	SDL_Rect src;
	SDL_Rect dest;

	int numSprites;
	int numSpritesinX;
	int numSpritesinY;

	int duration;
	int spriteDuration;
	int cont;

	int dir;

	void CalcularResto();

public:


	Particle(Vector2D position, int dir, std::string id, Entity* ent);
	virtual ~Particle();

	virtual void update();
	virtual void draw();
	virtual void draw(SDL_Rect* camera);

	bool dead = false;
};