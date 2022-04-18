﻿#include "PlayingState.h"
#include "GameOverState.h"
#include "../PlayingState/FightManager.h"
#include "../Entity.h"
#include "../Utils/Particle.h"
#include "../Utils/MyListener.h"
#include "../../../CharInclude.h"

PlayingState::PlayingState(FightManager* game, vector<int>player, vector<int>characters) : State(game) {
	std::vector<Entity*> entities;

	// input del character (al menos de momento):
	// 0 y 1: teclado
	// 2 y 3: mando NES
	// 4 y 5: mando PS4 o Xbox One
	int k = 0;
	int n = 0;
	int x = 0;

	for (auto i = 0u; i < player.size(); i++) {
		int control = 0;
		if (player[i] == 0 && k < 2) { //teclado
			control = k;
			k++;
		}
		else if (player[i] == 1 && n < 2) { //nes
			control = n + 2;
			n++;
		}
		else if (player[i] == 2 && x < 2) { // ps4 o xbox
			control = x + 4;
			x++;
		}
		switch (characters[i])
		{
		case 0: //zero
			entities.push_back(new CharacterZero(fmngr, new Vector2D(20+i * 10, 0), control));
			break;
		case 1://Gato espia

			entities.push_back(new GatoEspia(fmngr, new Vector2D(20+i*10, 0), control));
			break;
		case 2://Togo
			entities.push_back(new Togo(fmngr, new Vector2D(20+i * 10, 0), control));
			break;

		}
	}

	//entities.push_back(new Togo(fmngr, new Vector2D(20, 0), 2));
	//entities.push_back(new GatoEspia(fmngr, new Vector2D(30, 0), 1));

	fmngr->StartFight(entities);

}


void PlayingState::update() {
	fmngr->Update();
}

void PlayingState::draw() {

}

void PlayingState::next() {
    cout << "Next State " << endl;
    fmngr->setState(new GameOverState(fmngr));
    delete this;
}
