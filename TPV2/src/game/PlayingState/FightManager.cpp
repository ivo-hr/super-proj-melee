#include "FightManager.h"
#include "../Entity.h"
#include "../Utils/Particle.h"
#include "../Utils/MyListener.h"

FightManager::FightManager(SDLUtils* sdl, double screenAdjust) : world(b2World(b2Vec2(0.f, 15.f))), sdl(sdl)
{
	background = &sdl->images().at("fondo");
	testura = &sdl->images().at("testura");

	//Definimos un objeto (estatico)
	b2BodyDef groundDef;
	groundDef.position.Set(37.5f, 50.f);
	groundDef.type = b2_staticBody;

	//Anadimos al mundo
	stage = world.CreateBody(&groundDef);;
	//Le damos forma...
	b2PolygonShape floor;
	float floorW = 40.f, floorH = 30.f;
	floor.SetAsBox(floorW / 2, floorH / 2);

	//..cuerpo
	b2FixtureDef fixt;
	fixt.shape = &floor;
	fixt.density = 10.0f;
	fixt.friction = 0.5f;
	fixt.filter.categoryBits = 2; // 2 para el suelo principal
	//fixt.filter.maskBits = 1; // Colisiona con los personajes (tienen este categoryBits en Entity)

	stage->CreateFixture(&fixt);

	b2BodyDef gDef;
	gDef.position.Set(37.5f, 27.f);
	gDef.type = b2_staticBody;
	platform = world.CreateBody(&gDef);
	b2PolygonShape plat;
	float platW = 10.f, platH = 0.5f;
	plat.SetAsBox(platW / 2, platH / 2);
	b2FixtureDef fi;
	fi.shape = &plat;
	fi.density = 10.0f;;
	fi.friction = 0.5f;
	fi.filter.categoryBits = 4; // 4 para las plataformas que puedes atravesar desde abajo
	//fixt.filter.maskBits = 1; // Colisiona con los personajes (tienen este categoryBits en Entity)
	platform->CreateFixture(&fi);

	b2ToSDL = (sdl->width() * screenAdjust) / 75.f;

	//--------------------------

	//Creo las cajas que representaran a los objetos
	stageRect = GetSDLCoors(stage, floorW, floorH);
	platformRect = GetSDLCoors(platform, platW, platH);

	listener = new MyListener();
	world.SetContactListener(listener);

	deathZone = { 0, 0, (int)(sdl->width() * screenAdjust), (int)(sdl->height() * screenAdjust)};

	setState(new MenuState(this));
	while (!exit_) {
		ih.refresh();
		if (ih.isKeyDown(SDLK_ESCAPE))
			exit_ = true;
		getState()->update();
		getState()->draw();
	}
}

FightManager::~FightManager()
{
}

void FightManager::Update()
{

	Uint32 startTime = sdl->currRealTime();

	if (ih.isKeyDown(SDLK_ESCAPE))
		exit_ = true;
	if (ih.isKeyDown(SDLK_p) && ih.keyDownEvent()) {
		if (getSavedState() == nullptr) {
			//pause
			std::cout << "pause" << std::endl;
			saveState(getState());
			setState(new PauseState(this));
			return;
		}
	}
	//Esto llama al mundo para que simule lo que pasa en el tiempo que se le pase (en este caso 1000.f/60.f (un frame a 60 fps))
	float step = 1.f / 60.f;
	world.Step(step, 1, 1);

	sdl->clearRenderer(SDL_Color(build_sdlcolor(0xffffffff)));

	//Calculamos la posicion del sdl rect con respecto a las coordenadas que nos da box2d
	background->render(deathZone);
	testura->render(platformRect);
	testura->render(stageRect);
	//Dibujamos las cajas
	SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
	SDL_RenderDrawRect(sdl->renderer(), &stageRect);
	SDL_RenderDrawRect(sdl->renderer(), &platformRect);
	SDL_RenderDrawRect(sdl->renderer(), &deathZone);


	for (Particle* part : particulas)
	{
		part->draw();
		part->update();
	}
	for (auto i = 0u; i < entities.size(); i++)
	{
		entities[i]->update();
	}
	for (Entity* ent : entities)
	{
		ent->CheckHits();
	}
	for (int i = entities.size() - 1; i >= 0; i--)
	{
		entities[i]->draw();
	}

	while (addedDelay > 0)
	{

		Uint32 startTime = sdl->currRealTime();
		addedDelay--;

		// clear screen
		sdl->clearRenderer(SDL_Color(build_sdlcolor(0xffffffff)));
		//Calculamos la posicion del sdl rect con respecto a las coordenadas que nos da box2d
		background->render(deathZone);
		testura->render(platformRect);
		testura->render(stageRect);
		//Dibujamos las cajas
		SDL_SetRenderDrawColor(sdl->renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl->renderer(), &stageRect);
		SDL_RenderDrawRect(sdl->renderer(), &platformRect);
		SDL_RenderDrawRect(sdl->renderer(), &deathZone);

		for (Particle* part : particulas)
		{
			part->draw();
			part->update();
		}
		for (Entity* ent : entities)
		{
			ent->updateParticles();
		}
		for (int i = entities.size() - 1; i >= 0; i--)
		{
			entities[i]->draw();
		}
		// present new frame
		sdl->presentRenderer();

		double frameTime = sdl->currRealTime() - startTime;

		if (frameTime < (step * 1000))
		{
			SDL_Delay((step * 1000));
		}
	}

	// present new frame
	sdl->presentRenderer();

	double frameTime = sdl->currRealTime() - startTime;

	if (frameTime < (step * 1000))
	{
		SDL_Delay((step * 1000));
	}

	addedDelay = 0;
}

int FightManager::StartFight(std::vector<Entity*> ent)
{
	
	entities = ent;

	for (auto e : entities) {
		e->SetOponents(entities);
		listener->AddCharacter(e);
	}
	sdl->musics().at("running_grass").play();
	Music::setMusicVolume(20);

	return 1;
}

void FightManager::AddEntity(Entity* ent)
{
	entities.push_back(ent);
}

bool FightManager::RemoveEntity(Entity* ent)
{
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i] == ent)
		{
			for (int j = i + 1; j < entities.size(); j++)
			{
				entities[j - 1] = entities[j];
			}
			entities.pop_back();
		}
	}
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->DeleteOponent(ent);
	}
	delete ent;
	return false;
}

void FightManager::MoveToFront(Entity* ent)
{
	for (auto i = 0u; i < entities.size(); i++)
	{
		if (entities[i] == ent)
		{
			for (int j = i; j > 0; j--)
			{
				entities[j] = entities[j - 1];
			}
			entities[0] = ent;
			return;
		}
	}
}

void FightManager::AddParticle(Particle* par)
{
	particulas.push_back(par);
}

bool FightManager::RemoveParticle(Particle* par)
{
	for (int i = 0; i < particulas.size(); i++)
	{
		if (particulas[i] == par)
		{
			for (int j = i + 1; j < particulas.size(); j++)
			{
				particulas[j - 1] = particulas[j];
			}
			particulas.pop_back();
		}
	}
	delete par;
	return false;
}

void FightManager::HitLag(int frames)
{
	if (addedDelay < frames)
		addedDelay = frames;
}

void FightManager::KillingBlow(Vector2D dead)
{
	addedDelay = 40;
	AddParticle(new Particle(
		&dead,
		1, "killVfx", this));
	AddParticle(new Particle(
		&dead,
		1, "killHit", this));
}

void FightManager::FighterLost(Entity* loser)
{
	RemoveEntity(loser);
	numPlayers--;

	if (numPlayers == 1)
	{
		fightEnded = true;
		winner = entities[0];
	}
}

std::vector<Entity*> FightManager::GetOponents(Entity* current)
{
	std::vector<Entity*> a;

	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i] != current)
		{
			a.push_back(entities[i]);
		}
	}
	return a;
}

SDL_Rect FightManager::GetSDLCoors(b2Body* body, float width, float height)
{
	return { (int)((body->GetPosition().x * b2ToSDL) - (width * b2ToSDL) / 2.f),
		(int)((body->GetPosition().y * b2ToSDL) - (height * b2ToSDL) / 2.f),
		(int)(width * b2ToSDL),
		(int)(height * b2ToSDL) };
}

SDL_Rect FightManager::GetSDLCoors(float x, float y, float width, float height)
{
	return { (int)((x * b2ToSDL) - (width * b2ToSDL) / 2.f),
		(int)((y * b2ToSDL) - (height * b2ToSDL) / 2.f),
		(int)(width * b2ToSDL),
		(int)(height * b2ToSDL) };
}

int FightManager::b2ToSDLX(b2Body* body, float width)
{
	return (int)(body->GetPosition().x * b2ToSDL - width * b2ToSDL / 2);
}

int FightManager::b2ToSDLY(b2Body* body, float height)
{
	return (int)(body->GetPosition().y * b2ToSDL - height * b2ToSDL / 2);
}
