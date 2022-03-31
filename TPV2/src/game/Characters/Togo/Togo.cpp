#include "Togo.h"
#include "../../Utils/AnimationManager.h"
#include "../../../json/json.hpp"
#include "Spear.h"
#include <fstream>
#include <iostream>
using json = nlohmann::json;

Togo::Togo(FightManager* mngr, Vector2D* pos, char input) : Character(mngr, pos, input, 1.5f, 3.5f)
{

	ReadJson("resources/config/dino.json");
	//guardamos la textura
	texture = &sdl->images().at("blinkMaster");
	//smolH = &sdl->soundEffects().at("zeroSmolHit");

	anim = new AnimationManager(this, texture, spData);
}

Togo::~Togo()
{

}

//Lo mismo que el de arriba pero mas rapido y debil xd
void Togo::BasicNeutral(int frameNumber)
{
	if (frameNumber == 0)
	{
		anim->StartAnimation("basicN");
		//sdl->soundEffects().at("catAtk0").play();
	}
	else if (frameNumber == attacks["basicN"].startUp)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.h /= 6;


		hitbox.x += hitbox.w / 2;
		hitbox.y += hitbox.h / 2;

		if (dir == -1) {
			hitbox.x = hitbox.x - 60 - hitbox.w;
		}
		else if (dir == 1) {
			hitbox.x += 60;
		}
		hitbox.y += (hitbox.h+10);

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicN"], 2, OnHitData(5, false, false)));
	}
	else if (frameNumber == attacks["basicN"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}
void Togo::BasicForward(int frameNumber)
{
	if (frameNumber == 0)
	{
		if (!lanza) {
			currentMove = nullptr;
			moveFrame = -1;
			return;
		}
		anim->StartAnimation("basicF");
		//sdl->soundEffects().at("catAtk0").play();
	}
	else if (frameNumber == attacks["basicF"].startUp)
	{
			auto spear = new Spear(manager, new Vector2D(body->GetPosition().x, body->GetPosition().y), attacks["basicF"], b2Vec2(dir, 0), this);
			manager->AddEntity(spear);
			spear->SetOponents(oponents);
			SetSpear(false);
	}
	else if (frameNumber == attacks["basicN"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}
void Togo::BasicUpward(int frameNumber)
{
	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicU");
		sdl->soundEffects().at("catAtk3").play();
	}
	else if (frameNumber == attacks["basicU"].startUp)
	{
		body->SetLinearVelocity({ 0, body->GetLinearVelocity().y / 10 });
		if (!onGround) {
			body->SetGravityScale(0.0f);
		}
		attackBase = attacks["basicU"].base;
		attackMul = attacks["basicU"].multiplier;
		attacks["basicU"].base = 0;
		attacks["basicU"].multiplier = 0;
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.w *= 2.4f;
		hitbox.h *= 0.7f;
		hitbox.x -= hitbox.w / 1.8;
		hitbox.y -= 20;
		hitbox.h = hitbox.h / 3;
		hitbox.w *= 1.5;

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicU"], 3, OnHitData(15, false, false)));
	}
	else if (frameNumber == attacks["basicU"].startUp+3)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.w *= 2.4f;
		hitbox.h *= 0.7f;
		hitbox.x -= hitbox.w / 2;
		hitbox.y -= 20;
		hitbox.h = hitbox.h / 3;
		hitbox.w *= 1.5;

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicU"], 3, OnHitData(15, false, false)));
	}
	else if (frameNumber == attacks["basicU"].startUp+6)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.w *= 2.4f;
		hitbox.h *= 0.7f;
		hitbox.x -= hitbox.w / 2;
		hitbox.y -= 20;
		hitbox.h = hitbox.h / 3;
		hitbox.w *= 1.5;

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicU"], 3, OnHitData(15, false, false)));
	}
	else if (frameNumber == attacks["basicU"].startUp+9)
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.w *= 2.4f;
		hitbox.h *= 0.7f;
		hitbox.x -= hitbox.w / 2;
		hitbox.y -= 20;
		hitbox.h = hitbox.h / 3;
		hitbox.w *= 1.5;

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicU"], 3, OnHitData(15, false, false)));
	}
	else if (frameNumber == attacks["basicU"].startUp+12)
	{
		attacks["basicU"].base = attackBase;
		attacks["basicU"].multiplier = attackMul;
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.w *= 2.4f;
		hitbox.h *= 0.7f;
		hitbox.x -= hitbox.w / 2;
		hitbox.y -= 20;
		hitbox.h = hitbox.h / 3;
		hitbox.w *= 1.5;

		hitboxes.push_back(new Hitbox(hitbox, attacks["basicU"], 3, OnHitData(15, false, false)));
		body->SetGravityScale(10.0f);
	}
	else if (frameNumber == attacks["basicU"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}
void Togo::BasicDownward(int frameNumber)
{

}

//--------------------------------------------------------------------------------------------------

void Togo::SpecialNeutral(int frameNumber)
{

}

void Togo::SpecialForward(int frameNumber)
{

}

void Togo::SpecialUpward(int frameNumber)
{

}

void Togo::SpecialDownward(int frameNumber)
{

}

void Togo::SetSpear(bool spear)
{
	lanza = spear;
}

