#pragma once

#include "PlayingState/FightManager.h"

class AnimationManager;

class myListener : public b2ContactListener
{
public:
	bool ground = true;
	void BeginContact(b2Contact* contact)
	{
		b2Body* one = contact->GetFixtureA()->GetBody();
		b2Body* two = contact->GetFixtureB()->GetBody();
		if (two->GetType() == b2_staticBody)
		{
			ground = true;
			std::cout << ground << std::endl;
		}
	}
	void EndContact(b2Contact* contact)
	{
		b2Body* one = contact->GetFixtureA()->GetBody();
		b2Body* two = contact->GetFixtureB()->GetBody();
		if (two->GetType() == b2_staticBody)
		{
			std::cout << "ground?" << ground << std::endl;
			ground = false;
			std::cout << "ground?" << ground << std::endl;


		}
	}
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{

	}
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{

	}
	bool CheckGround()
	{
		return ground;
	}
};

//TODO ESTO DEBERIA SALIR DE JSON TAMBIEN AAAAAAAAAA

struct animationData
{
	//Por cada animación...
	int iniSprite;		//En que frame empieza
	int totalSprites;	//Cuantos frames son
	int keySprite;		//Sprite de la animación en el que debe aparecer la hitbox (Con respecto al sprite inicial)
	int hitboxFrame;	//El frame en el que el juego crea la hitbox del ataque
	int totalFrames;	//Cuantos frames dura el movimiento en total (Hasta que se repita o acabe)
	bool loop;			//Si se debe loopear
};

struct spriteSheetData
{
	int leftOffset;		//Cuanto debe sobresalir la spritesheet a la izquierda (par que el dibujo coincida con la hurtbox)
	int upOffset;		//bruh
	int sizeXOffset;	//Que tan grande se tiene que hacer el sprite en X para que coincida
	int sizeYOffset;	//bruh

	int spritesInX;		//Cuantos frames hay en una fila de la spritesheet entera
	int spritesInY;		//Cuantos frames hay en una columna de la spritesheet entera

	std::vector<animationData> animations;
};

struct atackData 
{
	int damage;
	float multiplier;
};

class Entity
{

protected:

	spriteSheetData spData;
	AnimationManager* anim;

	Texture* texture;

	FightManager* manager;

	SDLUtils* sdl;

	b2Body* body;

	SDL_Rect hurtbox;
	float width = 6.f;
	float height = 6.f;

	std::vector<Entity*> oponents;

	bool onGround;

	myListener listener;
public:


	Entity(FightManager* mngr); 
	~Entity();

	virtual void update();
	virtual void draw();

	virtual void SetOponents(std::vector<Entity*> op);

	virtual float GetWidth() { return width; };
	virtual float GetHeight() { return height; };
	virtual void GetHit(atackData a, int dir) = 0;
	virtual SDL_Rect* GetHurtbox();
<<<<<<< Updated upstream
	//bool GetGround() { return onGround; };
	//void SetGround(bool ground);
=======
	virtual b2Body* GetBody() { return body; };

	Vector2D GetCenterSDL() { return Vector2D(hurtbox.x + (hurtbox.w / 2), hurtbox.y + (hurtbox.h / 2)); };

	void SetGround(bool ground);
	bool GetGround() { return onGround; };
	bool isProjectile() { return projectile; };

	virtual bool changeDir() { return false; };

	void resetHit();
	Texture* getTexture() { return texture; }
	//virtual void SendToHUD(Texture* tex);
>>>>>>> Stashed changes
};