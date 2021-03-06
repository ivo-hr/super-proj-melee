#pragma once

#include <SDL.h>
#include <iostream>
#include <box2d.h>
#include <string>
#include <vector>

#include "../../sdlutils/InputHandler.h"
#include "../../sdlutils/macros.h"
#include "../../sdlutils/SDLUtils.h"
#include "../State/StateMachine.h"
#include "../State/MenuState.h"
#include "../State/PlayingState.h"
#include "../State/GameOverState.h"
#include "../Utils/PlayerSelectRect.h"
#include "../State/ConfigurationState.h"



class Entity;
class Character;
class Particle;

class HUDManager;

class MyListener;

class Stage;

class FightManager :public StateMachine
{

	std::vector<Entity*> entities;
	std::vector<Character*> characters;

	// Team mode
	std::vector<Character*> team1;
	std::vector<Character*> team2;
	vector<vector<int>>team1DeadStats;
	vector<vector<int>>team2DeadStats;
	vector<Texture*>team1DeadTextures;
	vector<Texture*>team2DeadTextures;


	SDLUtils* sdl;

	InputHandler& ih = *InputHandler::instance();
	HUDManager* hud;

	Entity* winner;

	double screenAdjust;

	bool fightEnded = false;

	int numPlayers = 0;
	
	int addedDelay;

	bool exit_ = false;

	MyListener* listener;
	
	float step = 1.f / 60.f;

	Stage* stage;

	SDL_Rect camera; 
	SDL_Rect auxCam;

	int cameraOffset = 75;
	Vector2D camShake;
	uint16 shakeDuration;

	void MoveCamera();

	vector<Texture*>deadTextures; // Textures of dead characters for gameover stats

	int winnerInput;
	vector<vector<int>>gameStats;
	bool endGame=false;
	int endGameTimer=0;
	bool teammode = false;

	void startCount();
	int scount = 4;
	int startticks=0;
public:

	FightManager(SDLUtils* sdl, double screenAdjust);
	virtual ~FightManager();

	bool isExit() { return exit_; };
	bool isFinish() { return fightEnded; };
	void Update();

	void HideOutOfBounds();

	void LoadStage(std::string file);
	int StartFight(std::vector<Character*> ent);
	int StartFight(std::vector<Character*> team1, std::vector<Character*> team2);

	void AddEntity(Entity* ent);
	bool RemoveEntity(Entity* ent);
	bool RemoveCharacter(Character* character);
	void MoveToFront(Entity* ent);
	void AddOponnent(Entity* ent, Entity* ignore = nullptr);

	void KillingBlow();

	void FighterLost(Character* loser);

	SDL_Rect GetSDLCoors(b2Body* body, float width, float height);
	SDL_Rect GetSDLCoors(float x, float y, float width, float height);
	int b2ToSDLX(b2Body* body, float width);
	int b2ToSDLY(b2Body* body, float height);
	int ToSDL(float x);

	int GetActualWidth();
	int GetActualHeight();

	double GetScreenRatio();
	double GetScreeAdjust();

	vector<Texture*>getWinnersTextures() { return deadTextures; }

	std::vector<Entity*> GetOponents(Entity* current);
	std::vector<Entity*> GetEntities(Entity* current);
	SDL_Rect* GetDeathZone() ;
	b2World* GetWorld();
	SDLUtils* GetSDLU() { return sdl; };

	int getWinnerInput() { return winnerInput; }
	void addCharacterStats(Character* character);
	vector<vector<int>>getGameStats() { return gameStats; }

	void userExit() { exit_ = true; };

	void onNewGame();

	bool getTeammode() { return teammode; }

	void SetShake(const Vector2D& dir, uint16 duration);

};
