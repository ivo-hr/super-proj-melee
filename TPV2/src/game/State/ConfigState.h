#pragma once

#include "State.h"

class ConfigState : public State {
    Texture* background, *exp;
    int numOfplayer;
    Button* keyb, *nes, *xbox ,*play , *nextb ,*back;
    vector<int> player;
    vector<int> charact;
    int sel = 0;
    int key = 0;
    int nesn = 0;
    int xboxn = 0;
    bool charsel = false;
    bool searchGamepad = false;
    double ts(double i);//TO SCREEN
    int k = -1;
    vector<bool> usedPad;
public:

    ConfigState(FightManager* game) ;
    ~ConfigState() = default;

    void update() override;
    void draw() override;
    void next() override;

    string getStateName() const override {
        return "Config state";
    };
};