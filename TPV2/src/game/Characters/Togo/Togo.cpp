#include "Togo.h"
#include "../../Utils/AnimationManager.h"
#include "../../../json/json.hpp"
#include "../../Utils/Particle.h"
#include "../../../utils/CheckML.h"
#include "Spear.h"
#include "DinoShield.h"
#include <fstream>
#include <iostream>
using json = nlohmann::json;

Togo::Togo(FightManager* mngr, b2Vec2 pos, char input,int p) : Character(mngr, pos, input,p, 1.5f, 3.5f)
{

	spriteSheetData spData;

	ReadJson("resources/config/dino.json", spData);
	//guardamos la textura
	texture = &sdl->images().at("dinoSouls");
	portrait = &sdl->images().at("dinoSoulsSelect");

	anim = new AnimationManager(this, texture, spData);
}

Togo::~Togo()
{

}

//Lo mismo que el de arriba pero mas rapido y debil xd
void Togo::BasicNeutral(int frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == 0)
	{
		anim->StartAnimation("basicN");
		sdl->soundEffects().at("dinoAtk0").play();
	}
	else if (frameNumber == attacks["basicN"].keyFrames[0])
	{
		CreateHitBox(&attacks["basicN"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["basicN"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Togo::BasicForward(int frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == 0)
	{
		if (!lanza) {
			currentMove = nullptr;
			moveFrame = -1;
			return;
		}
		anim->StartAnimation("basicF");
		sdl->soundEffects().at("dinoAtk1").play();
	}
	else if (frameNumber == attacks["basicF"].keyFrames[0])
	{
			auto spear = new Spear(manager, b2Vec2(body->GetPosition().x, body->GetPosition().y-height/2), attacks["basicF"].hitBoxes[0].hitdata, b2Vec2(dir, 0), this);
			manager->AddEntity(spear);
			manager->MoveToFront(spear);
			spear->SetOponents(oponents);
			SetSpear(false);
	}
	else if (frameNumber == attacks["basicF"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Togo::BasicUpward(int frameNumber)
{

	if (!onGround)
	{
		if (frameNumber > attacks["basicU"].keyFrames[0] && frameNumber < attacks["basicU"].keyFrames[4])
		{
			AllowMovement(0.4f);
		}
		else
		{
			AllowMovement(0.7f);
		}
	}

	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicU");
		sdl->soundEffects().at("dinoAtk2").play();
	}
	else if (frameNumber == attacks["basicU"].keyFrames[0])
	{
		body->SetLinearVelocity({ body->GetLinearVelocity().x / 2, body->GetLinearVelocity().y / 5 });

		speed /= 2;

		if (!onGround) {
			body->SetGravityScale(0.0f);
		}

		CreateHitBox(&attacks["basicU"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["basicU"].keyFrames[1])
	{
		CreateHitBox(&attacks["basicU"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["basicU"].keyFrames[2])
	{
		CreateHitBox(&attacks["basicU"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["basicU"].keyFrames[3])
	{
		CreateHitBox(&attacks["basicU"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["basicU"].keyFrames[4])
	{
		CreateHitBox(&attacks["basicU"].hitBoxes[1]);
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

	if (!onGround)
	{
		AllowMovement(0.7f);
	}

	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("basicD");
		sdl->soundEffects().at("dinoAtk3").play();
	}
	else if (frameNumber == attacks["basicD"].keyFrames[0])
	{
		CreateHitBox(&attacks["basicD"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["basicD"].keyFrames[1] - 2)
	{
		dir = -dir;
	}
	else if (frameNumber == attacks["basicD"].keyFrames[1])
	{
		CreateHitBox(&attacks["basicD"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["basicD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

//--------------------------------------------------------------------------------------------------

void Togo::SpecialNeutral(int frameNumber)
{
	if (onGround)
	{
		if (frameNumber == 0)
		{
			anim->StartAnimation("especialN");
			sdl->soundEffects().at("dinoSpecN").play();
			moving = false;
		}
		else if (frameNumber == attacks["specialN"].keyFrames[0])
		{
			anim->StartAnimation("especialNHold");
			dShield = new DinoShield(manager, b2Vec2(body->GetPosition().x, body->GetPosition().y - GetHeight() / 4.5));
			dShield->SetOponents(oponents);

			manager->AddEntity(dShield);

			manager->AddOponnent(dShield, this);

			manager->MoveToFront(dShield);

		}
		if (/*frameNumber == attacks["specialN"].totalFrames || */ !input->special())
		{
			anim->StartAnimation("idle");
			if (dShield != nullptr) {
				dShield->setToDelete();
				dShield = nullptr;
			}
			currentMove = nullptr;
			moveFrame = -1;
		}
	}
	else
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Togo::SpecialForward(int frameNumber)
{

	if (frameNumber < attacks["specialL"].keyFrames[0])
	{
		speed = dir * 34;
	}

	if (frameNumber <= 0) {
		anim->StartAnimation("especialL");
		sdl->soundEffects().at("dinoSpecS").play();
	}
	else if (frameNumber >= attacks["specialL"].keyFrames[0] && frameNumber < attacks["specialL"].keyFrames[1])
	{
		moving = false;
		body->SetLinearVelocity(b2Vec2(dir * 30, 0));
		speed = dir * 34;
		
		bite = manager->GetSDLCoors(
			body->GetPosition().x + (dir * width * 1.2f),
			body->GetPosition().y - height * 0.2f,
			width * 4,
			height * 0.7f);

		for (int i = 0; i < oponents.size(); i++) {
			if (SDL_HasIntersection(&bite, oponents[i]->GetHurtbox())) {
				ChangeMove([this](int f) { SpecialLHit(f); });
			}
		}

#ifdef _DEBUG

		SDL_RenderDrawRect(sdl->renderer(), &bite);

#endif // _DEBUG

	}
	else if (frameNumber == attacks["specialL"].keyFrames[1])
	{
		bite = { 0, 0, 0, 0 };
	}
	else if (frameNumber == attacks["specialL"].totalFrames)
	{
		recovery = false;
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Togo::SpecialUpward(int frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.4f);
	}

	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("especialU");
		sdl->soundEffects().at("dinoSpecU").play();
	}
	else if (frameNumber < attacks["specialU"].totalFrames) {
		if (frameNumber == attacks["specialU"].keyFrames[0])
		{
			CreateHitBox(&attacks["specialU"].hitBoxes[0]);

			body->SetGravityScale(0.0f);
			body->SetLinearVelocity(b2Vec2(0, -35));

		}
		if (body->GetLinearVelocity().x > 0 || body->GetLinearVelocity().x < 0) {
			body->SetLinearVelocity(b2Vec2(0, body->GetLinearVelocity().y));
		}
		
	}
	else if (frameNumber == attacks["specialU"].totalFrames)
	{
		body->SetGravityScale(10.0f);
		currentMove = nullptr;
		moveFrame = -1;
		recovery = false;
		//body->SetGravityScale(10);

	}
}

void Togo::SpecialDownward(int frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.2f);
	}

	if (frameNumber == 0)
	{
		moving = false;
		anim->StartAnimation("especialD");
		sdl->soundEffects().at("dinoSpecD").play();
	}
	else if (frameNumber == attacks["specialD"].keyFrames[0])
	{
		CreateHitBox(&attacks["specialD"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["specialD"].keyFrames[1] - 2)
	{
		dir = -dir;
	}
	else if (frameNumber == attacks["specialD"].keyFrames[1])
	{
		CreateHitBox(&attacks["specialD"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["specialD"].totalFrames)
	{
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Togo::SetSpear(bool spear)
{
	lanza = spear;
}

void Togo::SpecialLHit(int frameNumber)
{

	if (!onGround)
	{
		AllowMovement(0.4f);
	}

	if (frameNumber == 0) {
		anim->StartAnimation("especialLHit");
		moving = false;
		body->SetLinearVelocity(b2Vec2(0, 0));
	}
	else if (frameNumber == attacks["specialLHit"].keyFrames[0])
	{
		CreateHitBox(&attacks["specialLHit"].hitBoxes[0]);
	}
	else if (frameNumber == attacks["specialLHit"].totalFrames) {
		currentMove = nullptr;
		moveFrame = -1;
	}
}

void Togo::drawHUD(int numOfPlayer)
{
	Character::drawHUD(numOfPlayer);
}

bool Togo::GetHit(HitData a, Entity* attacker, bool& controlHitLag, bool& controlShake, bool& controlCamShake)
{
	if (dShield != nullptr)
	{
		dShield->setToDelete();
		dShield = nullptr;
	}
	return Character::GetHit(a, attacker, controlHitLag, controlShake, controlCamShake);
}

void Togo::update()
{
	if (dShield != nullptr && stun > 0)
	{
		dShield->setToDelete();
		dShield = nullptr;
	}
	Character::update();
}

void Togo::BuildBoxes()
{

	attacks["basicN"].hitBoxes[0].box = manager->GetSDLCoors(
		body->GetPosition().x + (dir * 2.f),
		body->GetPosition().y,
		width * 4.f,
		height * 0.4f);

	attacks["basicD"].hitBoxes[0].box = manager->GetSDLCoors(
		body->GetPosition().x + (dir * 1.8f),
		body->GetPosition().y + height * 0.3f,
		width * 2.6f,
		height * 0.5f);

	attacks["basicU"].hitBoxes[0].box = manager->GetSDLCoors(
		body->GetPosition().x,
		body->GetPosition().y - height * 0.7f,
		width * 3.7f,
		height * 0.3f);

	attacks["basicU"].hitBoxes[1].box = manager->GetSDLCoors(
		body->GetPosition().x,
		body->GetPosition().y - height * 0.7f,
		width * 4.f,
		height * 0.4f);

	attacks["specialLHit"].hitBoxes[0].box = manager->GetSDLCoors(
		body->GetPosition().x + (dir * width * 1.2f),
		body->GetPosition().y - height * 0.2f,
		width * 4.5f,
		height * 0.9f);

	attacks["specialD"].hitBoxes[0].box = manager->GetSDLCoors(
		body->GetPosition().x + (dir * 1.9f),
		body->GetPosition().y + height * 0.3f,
		width * 2.8f,
		height * 0.7f);

	attacks["specialU"].hitBoxes[0].box = manager->GetSDLCoors(
		body->GetPosition().x,
		body->GetPosition().y,
		width * 2,
		height * 1.2f);

}
