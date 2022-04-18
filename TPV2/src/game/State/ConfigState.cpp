#include "ConfigState.h"
#include "PlayingState.h"
#include "../PlayingState/FightManager.h"


ConfigState::ConfigState(FightManager* game) : State(game), numOfplayer(2) {
    background = &sdl->images().at("fondo");
    keyb = new Button(&sdl->images().at("fondo"), 0, 0, 100, 100);
    nes = new Button(&sdl->images().at("pause"), 0, 0, 100, 100);
    xbox = new Button(&sdl->images().at("star"), 0, 0, 100, 100);
    play = new Button(&sdl->images().at("play"), 1500, 900, 200, 100);
    player.resize(2);
    charact.resize(2);
}

void ConfigState::update() {
    
     if (ih.isKeyDown(SDLK_LEFT) && ih.keyDownEvent() && numOfplayer > 2) {
        sel = 0;
        numOfplayer--;
        player.resize(numOfplayer);
        charact.resize(numOfplayer);
        play->setUnrendered();
    }
    else if (ih.isKeyDown(SDLK_RIGHT) && ih.keyDownEvent() && numOfplayer < 4) {
        sel = 0;
        numOfplayer++;
        player.resize(numOfplayer);
        charact.resize(numOfplayer);
        play->setUnrendered();
    }
    if (keyb->mouseClick()) {
        player[sel] = 0;
        charact[sel] = 0;
        if(sel<numOfplayer)
        sel++;
    }
    else if(nes->mouseClick())
    {
        player[sel] = 1;
        charact[sel] = 1;
        if (sel < numOfplayer)
        sel++;
    }
    else if (xbox->mouseClick()) {
        player[sel] = 2;
        charact[sel] = 2;
        if (sel < numOfplayer)
        sel++;
    }
    if (play->mouseClick())fmngr->getState()->next();
}

void ConfigState::draw() {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    sdl->clearRenderer(SDL_Color(build_sdlcolor(0x0)));
    background->render({ 0,0,fmngr->GetActualWidth(),fmngr->GetActualHeight() });
    
    for (auto i = 0u; i < numOfplayer; i++) {
        
        showText(" Player "+ to_string(i+1) , 48 , 400, h/2-400+i*200, build_sdlcolor(0x112233ff));
        if (i == sel) {
            keyb->setX(400 + 200); keyb->setY(h / 2 - 400 + i * 200);
            nes->setX(400 + 400); nes->setY(h / 2 - 400 + i * 200);
            xbox->setX(400 + 600); xbox->setY(h / 2 - 400 + i * 200);
            keyb->render();
            nes->render();
            xbox->render();
        }
    }
    if(sel==numOfplayer)
    play->render();
    sdl->presentRenderer();
}

void ConfigState::next() {
    cout << "Next State " << endl;
    fmngr->setState(new PlayingState(fmngr,player,charact));
    delete this;
}
