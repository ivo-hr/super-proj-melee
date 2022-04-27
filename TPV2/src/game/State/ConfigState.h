#pragma once

#include "State.h"
#include "../Utils/PlayerPointer.h"
#include "../Utils/PlayerSelectRect.h"

class ConfigState : public State {
    Texture* background;
    int numOfplayer;
    Button *zero, *gatoespia, *maketo ,*togo , *nasnas ,*aleatorio , *plusB, *minusB ,*play;

    bool charsel = false;
    bool ready = false;
    bool searchGamepad = false;
    bool keyRelease = true;
    int lastPointerClick = -3;

    vector<int> playerInput;
    vector<int> charactersSelect;
    vector<bool> usedPad;
    vector<bool> usedKeyboard;
    vector<bool> selected;
    vector<PlayerPointer*> playerPointers;
    vector<PlayerSelectRect*> playerTexture;
    vector<Texture*> charactTexture;
    
public:

    ConfigState(FightManager* game , int fInput) ;
    ~ConfigState();

    void update() override;
    void draw() override;
    void next() override;

    string getStateName() const override {
        return "Config state";
    };
};