#include <stdio.h>
#include <SFML/Graphics.hpp>

#include "GameState.h"
#include "Engine.h"

#define FRAMERATE		60

bool Engine::Init(const char* title, int width, int height)
{
    // Create the game window
    window.create(sf::VideoMode(1280, 720), title);
	
    // start running the game
	running = true;

    frameTimeLimit = sf::seconds(1.0 / 60.0);
    clock.restart();

	printf("Engine Init\n");
	return true;
}

void Engine::Cleanup()
{
	// cleanup the all states
	while (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}

	printf("Engine Cleanup\n");
}

void Engine::ChangeState(GameState* state) 
{
	// cleanup the current state
	if (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init();
}

void Engine::PushState(GameState* state)
{
	// pause current state
	if (!states.empty()) {
		states.back()->Pause();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init();
}

void Engine::PopState()
{
	// cleanup the current state
	if ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// resume previous state
	if ( !states.empty() ) {
		states.back()->Resume();
	}
}


void Engine::HandleEvents() 
{
	states.back()->HandleEvents(this);
}

void Engine::Update() 
{
    if(!window.isOpen()) {
        running = false;
        return;
    }

	states.back()->Update(this);

    sleep(frameTimeLimit - clock.getElapsedTime());
    clock.restart();
}

void Engine::Draw() 
{
    window.clear(sf::Color(255, 255, 255));
	states.back()->Draw(this);
    window.display();
}
