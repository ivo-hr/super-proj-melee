#pragma once

#include "State.h"

class MenuState : public State {
    Texture* background;
    Button* playBut;
    int Finput = -3;
    Uint32 textTimer = 0;
    bool drawText = false;
public:

    MenuState(FightManager* game);
    ~MenuState() = default;

    void update() override;
    void draw() override;
    void next() override;

    string getStateName() const override {
        return "Menu state";
    };
};
