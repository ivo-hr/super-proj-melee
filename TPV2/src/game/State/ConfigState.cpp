#include "ConfigState.h"
#include "PlayingState.h"
#include "../PlayingState/FightManager.h"


ConfigState::ConfigState(FightManager* game , int fInput) : State(game), numOfplayer(2) {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    background = &sdl->images().at("selectbg");
    aleatorio = nullptr;
    nasnas = nullptr;
    zero = new Button(&sdl->images().at("zeroSelect"), ts(30), ts(50), ts(30), ts(30));
    gatoespia = new Button(&sdl->images().at("blinkMasterSelect"), ts(90), ts(50), ts(30), ts(30));
    maketo = new Button(&sdl->images().at("maktSelect"), ts(150), ts(50), ts(30), ts(30));
    togo = new Button(&sdl->images().at("dinoSoulsSelect"), ts(210), ts(50), ts(30), ts(30));
    plusB = new Button(&sdl->images().at("pB"), ts(480), ts(210), ts(30), ts(30));
    minusB = new Button(&sdl->images().at("mB"), ts(480), ts(240), ts(30), ts(30));
    play = new PlayButton(&sdl->images().at("play"), 0, 0, w, h);
    normalmode = new Button(&sdl->images().at("MNormalC"), &sdl->images().at("MNormalB"), ts(350), ts(2), ts(40), ts(20));
    normalmode->active(true);
    teammode = new Button(&sdl->images().at("MTeamC"), &sdl->images().at("MTeamB"), ts(400), ts(2), ts(40), ts(20));
   // play = new PlayButton(&sdl->images().at("play"), ts(130), ts(40), ts(250), ts(150));
    configTeamChoose();
    playerTexture.push_back(new PlayerSelectRect(&sdl->images().at("P1")));
    playerTexture.push_back(new PlayerSelectRect(&sdl->images().at("P2")));
    playerTexture.push_back(new PlayerSelectRect(&sdl->images().at("P3")));
    playerTexture.push_back(new PlayerSelectRect(&sdl->images().at("P4")));

    usedKeyboard.resize(2);
    playerInput.resize(1);
    playerInput[0] = fInput;
    playerTexture[0]->setgotInput(true);
    charactersSelect.resize(2);
    usedPad.resize(SDL_NumJoysticks());
    selected.resize(4);

    if (fInput >= 0) { usedPad[fInput] = true; playerTexture[0]->setFront(&sdl->images().at("Mando")); }
    else if (fInput == -1) { usedKeyboard[0] = true; playerTexture[0]->setFront(&sdl->images().at("k1"));
    }
    else if (fInput == -2) { usedKeyboard[1] = true; playerTexture[0]->setFront(&sdl->images().at("k2"));
    }

    int pointerSize = 15;
    int dist = (w - ts(50)) / numOfplayer;
    int offset = dist - ts(60);
    playerPointers.push_back(new PlayerPointer(&sdl->images().at("P1P"), 0u * dist + offset, 676, ts(pointerSize), ts(pointerSize),w,h));
    playerPointers.push_back(new PlayerPointer(&sdl->images().at("P2P"), 1u, 676, ts(pointerSize), ts(pointerSize), w, h));
    playerPointers.push_back(new PlayerPointer(&sdl->images().at("P3P"), 2u, 676, ts(pointerSize), ts(pointerSize), w, h));
    playerPointers.push_back(new PlayerPointer(&sdl->images().at("P4P"), 3u, 676, ts(pointerSize), ts(pointerSize), w, h));
    playerPointers[0]->setActive(true);
    sdl->musics().at("sawtines").play();
}

ConfigState::~ConfigState()
{
    delete zero;
    delete gatoespia;
    delete maketo;
    delete togo;
    delete nasnas;
    delete aleatorio;
    delete plusB;
    delete minusB;
    delete play;
    delete teammode;
    delete normalmode;
    for (auto e : playerPointers)delete e;
    for (auto e : playerTexture)delete e;
    for (auto e : charactTexture)delete e;
}

void ConfigState::update() {
    //Si hay algun input pendiente , buscarlo
    if (playerInput.size()<numOfplayer) {
        for (auto i = 0u; i < SDL_NumJoysticks(); i++) {
            if (ih.xboxGetAxesState(i, 1) == -1 && !usedPad[i]) {
                usedPad[i] = true;
                playerInput.push_back(i);
                setPointer();
                playerTexture[playerInput.size() - 1]->setgotInput(true);
                playerTexture[playerInput.size() - 1]->setFront(&sdl->images().at("Mando"));
                charactersSelect.resize(playerInput.size());
                return;
            }
        }
        if (ih.isKeyDown(SDLK_UP) && !usedKeyboard[1]) {
            usedKeyboard[1] = true;
            playerInput.push_back(-2);
            setPointer();
            playerTexture[playerInput.size() - 1]->setgotInput(true);
            playerTexture[playerInput.size() - 1]->setFront(&sdl->images().at("k2"));
        }
        if (ih.isKeyDown(SDLK_w)&& !usedKeyboard[0]) {
            usedKeyboard[0] = true;
            playerInput.push_back(-1);
            setPointer();
            playerTexture[playerInput.size() - 1]->setgotInput(true);
            playerTexture[playerInput.size() - 1]->setFront(&sdl->images().at("k1"));
        }
        charactersSelect.resize(playerInput.size());
    }
    //Movimiento de los punteros
    for (auto i = 0; i < playerInput.size(); i++) {
        switch (playerInput[i])
        {
        case -1:
            if (ih.isKeyDown(SDLK_w))playerPointers[i]->move(0);
            if (ih.isKeyDown(SDLK_s))playerPointers[i]->move(1);
            if (ih.isKeyDown(SDLK_a))playerPointers[i]->move(2);
            if (ih.isKeyDown(SDLK_d))playerPointers[i]->move(3);
            break;
        case -2:
            if (ih.isKeyDown(SDLK_UP))playerPointers[i]->move(0);
            if (ih.isKeyDown(SDLK_DOWN))playerPointers[i]->move(1);
            if (ih.isKeyDown(SDLK_LEFT))playerPointers[i]->move(2);
            if (ih.isKeyDown(SDLK_RIGHT))playerPointers[i]->move(3);
            break;
        default:
            if(ih.xboxGetAxesState(playerInput[i], 1) == -1 || ih.xboxGetDpadState(playerInput[i], 0))playerPointers[i]->move(0);
            if(ih.xboxGetAxesState(playerInput[i], 1) == 1 || ih.xboxGetDpadState(playerInput[i], 2))playerPointers[i]->move(1);
            if(ih.xboxGetAxesState(playerInput[i], 0) == -1 || ih.xboxGetDpadState(playerInput[i] ,3))playerPointers[i]->move(2);
            if(ih.xboxGetAxesState(playerInput[i], 0) == 1 || ih.xboxGetDpadState(playerInput[i], 1))playerPointers[i]->move(3);
            break;
        }
    }
    //Comprobacion de punteros con los botones
    for (auto i = 0; i < playerInput.size(); i++) {
        bool enter = false;
        switch (playerInput[i])
        {
        case -1:
            if (ih.isKeyDown(SDLK_e))enter = true;
            break;
        case -2:
            if (ih.isKeyDown(SDLK_l))enter = true;
            break;
        default:
            if( ih.xboxGetButtonState(playerInput[i], SDL_CONTROLLER_BUTTON_B))enter = true;
            break;
        }
        if (zero->pointerClick(playerPointers[i]->getRect())&&enter && keyRelease && !selected[i]) {
            playerTexture[i]->setFront(&sdl->images().at("zeroSelect"));
            charactersSelect[i] = 0;
            keyRelease = false;
            lastPointerClick = playerInput[i];
            selected[i] = true;
        }
        else if (gatoespia->pointerClick(playerPointers[i]->getRect()) && enter && keyRelease && !selected[i]) {
            playerTexture[i]->setFront(&sdl->images().at("blinkMasterSelect"));
            charactersSelect[i] = 1;
            keyRelease = false;
            lastPointerClick = playerInput[i];
            selected[i] = true;
        }
        else if (togo->pointerClick(playerPointers[i]->getRect()) && enter && keyRelease && !selected[i]) {
            playerTexture[i]->setFront(&sdl->images().at("dinoSoulsSelect"));
            charactersSelect[i] = 2;
            keyRelease = false;
            lastPointerClick = playerInput[i];
            selected[i] = true;
        }
        else if (maketo->pointerClick(playerPointers[i]->getRect()) && enter && keyRelease && !selected[i]) {
            playerTexture[i]->setFront(&sdl->images().at("maktSelect"));
            charactersSelect[i] = 3;
            keyRelease = false;
            lastPointerClick = playerInput[i];
            selected[i] = true;
        }
        else if (minusB->pointerClick(playerPointers[i]->getRect()) && enter && numOfplayer > 2 && keyRelease) {
            numOfplayer--;

            if (playerInput.size() > numOfplayer) {
                switch (playerInput[playerInput.size() - 1])
                {
                case -1:
                    usedKeyboard[0] = false;
                    break;
                case -2:
                    usedKeyboard[1] = false;
                    break;
                default:
                    usedPad[playerInput[playerInput.size() - 1]] = false;
                    break;
                }
                playerInput.resize(numOfplayer);
                playerPointers[playerInput.size()]->setActive(false);
                playerTexture[playerInput.size()]->setgotInput(false);
            }
            charactersSelect.resize(playerInput.size());
            keyRelease = false;
            lastPointerClick = playerInput[i];
        }
        else if (plusB->pointerClick(playerPointers[i]->getRect()) && enter && numOfplayer < 4 && keyRelease) {
            numOfplayer++;
            charactersSelect.resize(playerInput.size());
            keyRelease = false;
            lastPointerClick = playerInput[i];
        }
        else if (normalmode->pointerClick(playerPointers[i]->getRect()) && enter  && keyRelease) {
            TeamModebool = false;
            normalmode->active(true);
            teammode->active(false);
        }
        else if (teammode->pointerClick(playerPointers[i]->getRect()) && enter && keyRelease) {
            TeamModebool = true;
            normalmode->active(false);
            teammode->active(true);
        }
        for (auto j = 0u; j < 2; j++) {
            if (p[i][j]->pointerClick(playerPointers[i]->getRect()) && enter && keyRelease) {
                if (j == 0) {
                    p[i][1]->active(false);
                    p[i][0]->active(true);
                    charactersTeam[i] = 0;
                }
                else
                {
                    p[i][1]->active(true);
                    p[i][0]->active(false);
                    charactersTeam[i] = 1;
                }
            }
        }
        
        switch (lastPointerClick)
        {
        case -1:
            if (!ih.isKeyDown(SDLK_e))keyRelease = true;
            break;
        case -2:
            if (!ih.isKeyDown(SDLK_l))keyRelease = true;
            break;
        case -3:
            break;
        default:
            if (!ih.xboxGetButtonState(lastPointerClick, SDL_CONTROLLER_BUTTON_B))keyRelease = true;
            break;
        }
    }
    for (auto i = 0; i < playerInput.size(); i++) {
        switch (playerInput[i])
        {
        case -1:
            if (ih.isKeyDown(SDLK_r))selected[i] = false;
            break;
        case -2:
            if (ih.isKeyDown(SDLK_k))selected[i] = false;
            break;
        default:
            if (ih.xboxGetButtonState(playerInput[i], SDL_CONTROLLER_BUTTON_A))selected[i] = false;
            break;
        }
    }
    //Para +- numPlayer con el raton
    if (minusB->mouseClick()  && numOfplayer > 2) {
        numOfplayer--;

        if (playerInput.size()>numOfplayer) {
            switch (playerInput[playerInput.size() - 1])
            {
            case -1:
                usedKeyboard[0] = false;
                break;
            case -2:
                usedKeyboard[1] = false;
                break;
            default:
                usedPad[playerInput[playerInput.size() - 1]] = false;
                break;
            }
            playerInput.resize(numOfplayer);
            playerPointers[playerInput.size()]->setActive(false);
            playerTexture[playerInput.size()]->setgotInput(false);
        }
        charactersSelect.resize(playerInput.size());
    }
    else if (plusB->mouseClick()  && numOfplayer < 4) {
        numOfplayer++;
        charactersSelect.resize(playerInput.size());
    }
    else if (normalmode->mouseClick() ) {
        TeamModebool = false;
        normalmode->active(true);
        teammode->active(false);
    }
    else if (teammode->mouseClick() ) {
        TeamModebool = true;
        normalmode->active(false);
        teammode->active(true);
    }
    for (auto i = 0u; i < playerInput.size(); i++) {
        for (auto j = 0u; j < 2; j++) {
            if (p[i][j]->mouseClick()) {
                if (j == 0) {
                    p[i][1]->active(false);
                    p[i][0]->active(true);
                    charactersTeam[i] = 0;
                }
                else
                {
                    p[i][1]->active(true);
                    p[i][0]->active(false);
                    charactersTeam[i] = 1;
                }
            }
        }
    }


    //Empezar la partida
    for (auto i = 0u; i < numOfplayer; i++) {
        if (!selected[i]) {
            ready = false;
            play->reset();
            return;
        }
        ready = true;
    }

    if (ready) {
        if (play->mouseClick())fmngr->getState()->next();
        for (auto i = 0; i < playerInput.size(); i++) {
            bool enter = false;
            switch (playerInput[i])
            {
            case -1:
                if (ih.isKeyDown(SDLK_e))enter = true;
                break;
            case -2:
                if (ih.isKeyDown(SDLK_l))enter = true;
                break;
            default:
                if (ih.xboxGetButtonState(playerInput[i], SDL_CONTROLLER_BUTTON_B))enter = true;
                break;
            }
            if (play->pointerClick(playerPointers[i]->getRect()) && enter && keyRelease) {
                fmngr->getState()->next();
            }
        }
    }
 
  
    //if (ih.isKeyDown(SDLK_RETURN))fmngr->getState()->next();
}

void ConfigState::draw() {
    int w = fmngr->GetActualWidth();
    int h = fmngr->GetActualHeight();
    sdl->clearRenderer(SDL_Color(build_sdlcolor(0x0)));
    background->render({ 0,0,fmngr->GetActualWidth(),fmngr->GetActualHeight() });
    int dist = (w - ts(50)) / numOfplayer;
    int offset = dist - ts(110);
    for (auto i = 0u; i < numOfplayer; i++) {
 
        playerTexture[i]->render((int)(i*dist + offset), (int)ts(200), (int)ts(110), (int)ts(80));
        if(selected[i])
        showText("Selected", ts(8), (int)(i * dist + offset + ts(30)), (int)ts(250), build_sdlcolor(0x00FF0000));  
    }

    if (TeamModebool) {
        for (auto i = 0u; i < playerInput.size(); i++) {

            for (int j = 0; j < 2; j++)
            {
                p[i][j]->setX((i * dist + offset + ts(30) + j * ts(30)));
                p[i][j]->setY((int)ts(260));
                p[i][j]->render();
            }
        }
    }
    zero->render();
    gatoespia->render();
    togo->render();
    maketo->render();
    plusB->render();
    minusB->render();
    normalmode->render();
    teammode->render();
    if (ready)
        play->render();
    for (auto e : playerPointers)e->render();
    sdl->presentRenderer();
}

void ConfigState::next() {
    cout << "Next State " << endl;
    if (!TeamModebool) {
        fmngr->setState(new PlayingState(fmngr, playerInput, charactersSelect));
    }
    else
    {
        fmngr->setState(new PlayingState(fmngr, playerInput, charactersSelect,charactersTeam));
    }
    delete this;
}

void ConfigState::setPointer()
{
    int w = fmngr->GetActualWidth();
    int dist = (w - ts(50)) / numOfplayer;
    int offset = dist - ts(60);
    playerPointers[playerInput.size() - 1]->setActive(true);
    playerPointers[playerInput.size() - 1]->setPosition((playerInput.size() - 1) * dist + offset, 676);
}
void ConfigState::configTeamChoose()
{
    charactersTeam.resize(4);
    for (int i = 0; i < 4; i++)
    {
        vector<Button*> p1;
        p1.push_back(new Button(&sdl->images().at("T1C"), &sdl->images().at("T1B"), ts(350), ts(2), ts(20), ts(20)));
        p1.push_back(new Button(&sdl->images().at("T2C"), &sdl->images().at("T2B"), ts(350), ts(2), ts(20), ts(20)));
        p.push_back(p1);
    }
    for (int i = 0; i < 4; i++) {
        p[i][0]->active(true);
        charactersTeam[i] = 0;
    }
}