#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <chipmunk/chipmunk.h>
#include <cmath>

#include "Engine.h"
#include "GMenuState.h"
#include "GPlayState.h"

GPlayState GPlayState::g_PlayState;

void GPlayState::Init()
{
    stepSize = 1.0 / 60.0;
    space = cpSpaceNew();
    cpSpaceSetIterations(space, 6);

	terrain = Terrain(space);

	player = new Character(space, 0, 0);
    characters.push_back(player);

    cam.setFocus(player);
	inputManager = InputManager(player);
    
    for(int i = 0; i < 3; i++) {
        characters.push_back(new Character(space, 350, 100 * i));
        AIs.push_back(new AIController(characters.back()));
    }

	printf("GPlayState Init\n");
}

void GPlayState::Cleanup()
{
	printf("GPlayState Cleanup\n");
    for(Character *c : characters) {
        if(c != NULL) delete c;
    }
    cpSpaceFree(space);
}

void GPlayState::Pause()
{
	printf("GPlayState Pause\n");
}

void GPlayState::Resume()
{
	printf("GPlayState Resume\n");
}

void GPlayState::HandleEvents(Engine* game)
{
    sf::Event ev;

    // wait for an event (mouse movement, key press, etc.)
    while(game->window.pollEvent(ev)) {
        switch(ev.type) {
            case sf::Event::Closed:
                game->window.close();
                game->Quit();
                break;

            case sf::Event::KeyPressed:
                inputManager.CheckKeyPress(ev.key.code);

                if(ev.key.code == sf::Keyboard::Escape)
                    Quit(game);

                break;

            case sf::Event::KeyReleased:
                inputManager.CheckKeyRelease(ev.key.code);
                break;

            case sf::Event::MouseMoved:
                break;

            case sf::Event::MouseButtonPressed:
                inputManager.mousePress(game);
                break;

            case sf::Event::MouseButtonReleased:
                break;
                
            default: break;
        }
    }
}

void GPlayState::Update(Engine* game)
{
    inputManager.Update();
    for(Character *c : characters)
        c->update();

    float step = stepSize / 3.0;
    for(int i = 0; i < 3; i++) {
        for(Character *c : characters)
            c->physUpdate();

        cpSpaceStep(space, step);
    }

    cam.update();
    game->window.setView(cam.getView());
}

void GPlayState::Quit(Engine* game)
{
    game->PopState();
}

Terrain *GPlayState::getTerrain()
{
    return &terrain;
}

void GPlayState::Draw(Engine* game)
{
    sf::Transform trans = sf::Transform::Identity;

    terrain.drawTerrain(&game->window, trans);

    for(Character *c : characters) {
        c->draw(&game->window, trans);
    }
}
