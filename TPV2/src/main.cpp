// This file is part of the course Proyectos2@UCM - LeftOvers Productions �

#include <SDL.h>
#include <iostream>
#include <box2d.h>
#include "game/Character.h"
#include "sdlutils/InputHandler.h"
#include "sdlutils/macros.h"

#include "sdlutils/sdlutils_demo.h"
#include "sdlutils/SDLUtils.h"



int main(int ac, char **av) {

	// Initialise the SDLGame singleton
	SDLUtils::init("Project Vs21", 1024, 576,
		"resources/config/resources.json");

	auto& sdl = *SDLUtils::instance();

	//Obtenemos el tama�o de la pantalla
	SDL_DisplayMode DM;
	SDL_GetDesktopDisplayMode(0, &DM);

	/*
	float scaleX = (float)DM.w / sdl.width();
	float scaleY = (float)DM.h / sdl.height();

	SDL_RenderSetScale(sdl.renderer(), scaleX, scaleY);
	*/

	//Escalamos toda la ventana para que se ajuste al tama�o de la pantalla
	SDL_RenderSetLogicalSize(sdl.renderer(), DM.w, DM.h);

	//Cambiamos el tama�o de la ventana
	SDL_SetWindowSize(sdl.window(), DM.w, DM.h);

	//Ponemos en pantalla completa
	//sdl.toggleFullScreen();

	//show the cursor
	sdl.showCursor();

	// reference to the input handler (we could use a pointer, I just . rather than ->).
	// you can also use the inline method ih() that is defined in InputHandler.h
	auto& ih = *InputHandler::instance();

	//-----------------------------------------------------------------------------------------

	//Creamos el espacio fisico
	b2Vec2 gravity = b2Vec2(0.0f, 20.0f);

	b2World world = b2World(gravity);

	Character* character1 = new Character(&world, &sdl, true);
	Character* boxingBag = new Character(&world, &sdl, false);
	character1->SetOponent(boxingBag);
	boxingBag->SetOponent(character1);

	//Definimos un objeto (estatico)
	b2BodyDef groundDef;
	groundDef.position.Set(96.0f, 90.0f);
	groundDef.type = b2_staticBody;

	//A�adimos al mundo
	b2Body* ground = world.CreateBody(&groundDef);;
	//Le damos forma...
	b2PolygonShape floor;
	floor.SetAsBox(75.0f, 5.0f);

	//..cuerpo
	b2FixtureDef fixt;
	fixt.shape = &floor;
	fixt.density = 10.0f;
	fixt.friction = 0.9f;

	ground->CreateFixture(&fixt);

	//--------------------------
	
	//Creo las cajas que representaran a los objetos
	SDL_Rect scene = { 960.0f - 750.0f, 900.0f - 50.f, 1500, 10 };

	int32 speed = 0;

	// a boolean to exit the loop
	bool exit_ = false;
	int32 x2;
	int32 y2;


	int iniJumps = 1;
	int eJumps = iniJumps;
	int moveBuffer=0;
	bool moveing = false;
	//Bucle que estaba en la demo pero modificado xd
	while (!exit_) {
		Uint32 startTime = sdl.currRealTime();


		if (ih.isKeyDown(SDLK_ESCAPE))
			exit_ = true;

		//Esto llama al mundo para que simule lo que pasa en el tiempo que se le pase (en este caso 1000.f/60.f (un frame a 60 fps))
		double step = 1.f / 60.f;
		world.Step(step, 1, 1);

		// clear screen
		sdl.clearRenderer(SDL_Color(build_sdlcolor(0xffffffff)));

		//Calculamos la posicion del sdl rect con respecto a las coordenadas que nos da box2d

		//Dibujamos las cajas
		SDL_SetRenderDrawColor(sdl.renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdl.renderer(), &scene);

		character1->update();
		boxingBag->update();
		character1->draw();
		boxingBag->draw();

		// present new frame
		sdl.presentRenderer();

		double frameTime = sdl.currRealTime() - startTime;

		if (frameTime < step)
		{
			SDL_Delay(step - frameTime);
		}
	}

	//std::cout << "Hello World!\n";
	//try {
	//	sdlutils_basic_demo();
	//} catch (const std::string &e) {		// catch exceptions thrown as strings
	//	std::cerr << e << std::endl;
	//} catch (const char *e) {				// catch exceptions thrown as char*
	//	std::cerr << e << std::endl;
	//} catch (const std::exception &e) {		// catch exceptions thrown as a sub-type of std::exception
	//	std::cerr << e.what();
	//} catch (...) {
	//	std::cerr << "Caught and exception of unknown type ...";
	//}


	return 0;
}
