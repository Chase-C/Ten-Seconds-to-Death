#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Engine.h"
#include "GameState.h"
#include "GInitState.h"
#include "GMenuState.h"

GInitState GInitState::g_InitState;

void GInitState::Init()
{
    if(!bg.loadFromFile("Logo.bmp")) {
        fprintf(stderr, "failed to load Logo.bmp\n");
    }
    sprite.setTexture(bg);
    sprite.setPosition(200, 300);

    // start off transparent
    alpha = 0;

    clock.restart();
    totalTime = sf::seconds(5);
    fadeTime = sf::seconds(1);

    printf("GInitState Init\n");
}

void GInitState::Cleanup()
{
    printf("GInitState Cleanup\n");
}

void GInitState::Pause()
{
    printf("GInitState Pause\n");
}

void GInitState::Resume()
{
    printf("GInitState Resume\n");
}

void GInitState::HandleEvents(Engine* game)
{
    // wait for an event (mouse movement, key press, etc.)
    sf::Event event;
    while(game->window.pollEvent(event)) {
        switch(event.type) {
            case sf::Event::Closed:
                game->window.close();
                game->Quit();
                break;

            case sf::Event::KeyPressed:
                game->ChangeState(GMenuState::Instance());
                break;

            default: break;
        }
    }
}

void GInitState::Update(Engine* game) 
{
    sf::Time dt = clock.getElapsedTime();

    if(dt <= fadeTime) {
        alpha = (255 * dt.asSeconds()) / fadeTime.asSeconds();
    } else if(dt >= totalTime - fadeTime) {
        alpha = 255 - ((255 * (dt - (totalTime - fadeTime)).asSeconds()) / fadeTime.asSeconds());
    } else {
        alpha = 255;
    }


    if (alpha > 255) 
        alpha = 255;
    if(dt > totalTime)
        game->ChangeState(GMenuState::Instance());

    sprite.setColor(sf::Color(255, 255, 255, alpha));
}

void GInitState::Draw(Engine* game) 
{
    game->window.draw(sprite);
}
