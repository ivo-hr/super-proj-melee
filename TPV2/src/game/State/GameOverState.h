#pragma once

#include "State.h"

class GameOverState : public State {

public:

    GameOverState(FightManager* fmngr);
    ~GameOverState() = default;

    void update() override;
    void draw() override;
    void next() override;

    string getStateName() const override {
        return "GameOver state";
    };
};