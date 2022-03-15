#include "CharacterZero.h"
#include "../../Utils/AnimationManager.h"

CharacterZero::CharacterZero(FightManager* mngr, Vector2D* pos, char input) : Character(mngr, pos, input)
{

	//guardamos la textura
	texture = &sdl->images().at("zero");
	//smolH = &sdl->soundEffects().at("zeroSmolHit");


	//Aqui defino las caracteristicas de cada hitbox (podriamos hacerlo dentro de cada metodo, y vendria de json)(tambien podr�amos poner framedata)
	ataqueFuerte.direction = b2Vec2(1, 2);
	ataqueFuerte.direction.Normalize();
	ataqueFuerte.base = 40;
	ataqueFuerte.damage = 20;
	ataqueFuerte.multiplier = 1.8f;

	ataqueDebil.direction = b2Vec2(1, 4);
	ataqueDebil.direction.Normalize();
	ataqueDebil.base = 20;
	ataqueDebil.damage = 5;
	ataqueDebil.multiplier = 0.2f;

	// variables
	weight = 10;
	damageTaken = 0;
	maxSpeed = 30;
	speed = 0;
	maxJumps = 1;
	jumpStr = 1500;
	jumpCounter = maxJumps;
	onGround = true;

	//Datos para las animaciones (tendr� que venir de json claramente solo hay tres y ya ocupan 37 lineas xd)

	//Mirando a la derecha

	spData.leftOffset = 4;		//Pixeles en sprite que se dibujaran fuera de la hurtbox a la izquierda
	spData.upOffset = 48;
	spData.sizeXOffset = 28;	//Cuantos pixeles en X NO estan dentro de la hurtbox
	spData.sizeYOffset = 48;

	spData.spritesInX = 5;
	spData.spritesInY = 4;

	animationData aux;

	aux.iniSprite = 0;
	aux.totalSprites = 2;
	aux.keySprite = -1;
	aux.hitboxFrame = -1;
	aux.totalFrames = 40;
	aux.loop = true;
	
	spData.animations.push_back(aux);

	aux.iniSprite = 2;
	aux.totalSprites = 4;
	aux.keySprite = 2;
	aux.hitboxFrame = 12;
	aux.totalFrames = 20;
	aux.loop = false;

	spData.animations.push_back(aux);

	aux.iniSprite = 6;
	aux.totalSprites = 13;
	aux.keySprite = 9;
	aux.hitboxFrame = 56;
	aux.totalFrames = 100;
	aux.loop = false;

	spData.animations.push_back(aux);

	aux.iniSprite = 19;
	aux.totalSprites = 1;
	aux.keySprite = -1;
	aux.hitboxFrame = -1;
	aux.totalFrames = 5;
	aux.loop = false;

	spData.animations.push_back(aux);

	anim = new AnimationManager(this, texture, spData);
}

CharacterZero::~CharacterZero()
{

}

void CharacterZero::draw()
{
	Character::draw();
}


void CharacterZero::BasicNeutral(int frameNumber)
{

	//Dependiendo del frame en el que est�, hara una cosa u otra..

	switch (frameNumber)
	{
	case 0:
		sdl->soundEffects().at("zeroBigHit").play();
		//Empieza el ataque :v
		anim->StartAnimation(2);
		break;
		//No hace nada, esto es el cargar el pu�o
		break;
	case 56:
	{
		//Al frame 90, crea un rect y si el oponente colisiona con ello...
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.w /= 2;
		hitbox.h /= 2;

		hitbox.x += hitbox.w / 2;

		hitbox.y += hitbox.h / 2;

		hitbox.x += dir * 60;

		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl->renderer(), &hitbox);

		for (int i = 0; i < oponents.size(); i++)
		{
			if (SDL_HasIntersection(&hitbox, oponents[i]->GetHurtbox()))
			{
				manager->HitLag(20);
				//Le hace da�o xddd
				oponents[i]->GetHit(ataqueFuerte, dir);
			}
		}
	}
	break;
	case 100:

		//Al ultimo frame...

		//Vacia current move para que Character sepa que ha acabado
		currentMove = nullptr;

		//Reinicia moveFrame para el siguiente
		moveFrame = -1;
		break;
	}
}



//Lo mismo que el de arriba pero mas rapido y debil xd
void CharacterZero::SpecialNeutral(int frameNumber)
{
	
	switch (frameNumber)
	{
	case 0:
		sdl->soundEffects().at("zeroSmolHit").play();
		anim->StartAnimation(1);
		break;
	case 12:
	{
		SDL_Rect hitbox = manager->GetSDLCoors(body, width, height);

		hitbox.x += dir * 30;

		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl->renderer(), &hitbox);

		for (int i = 0; i < oponents.size(); i++)
		{
			if (SDL_HasIntersection(&hitbox, oponents[i]->GetHurtbox()))
			{
				//Le hace da�o xddd
				oponents[i]->GetHit(ataqueDebil, dir);
			}
		}
	}
	break;
	case 20:
		currentMove = nullptr;
		moveFrame = -1;
		break;
	}
}
