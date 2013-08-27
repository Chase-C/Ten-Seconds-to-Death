#include <stdio.h>
#include <string.h>
#include <SFML/Graphics.hpp>

#include "Engine.h"
#include "GameState.h"
#include "GPlayState.h"
#include "GMenuState.h"
#include "GCharacterSelectState.h"

GCharacterSelectState GCharacterSelectState::g_CharacterSelectState;

// pass these to the menu items
void Play(Engine *game);
void HighScores(Engine *game);
void Quit(Engine *game);

void GCharacterSelectState::Init()
{
	p1Index = 0;
	p2Index = 0;

    p1LockIn = false;
    p2LockIn = false;

    bgT = new sf::Texture();
    playerMarkers = new sf::Texture();

    if(!bgT->loadFromFile("rec/selectBG.png")) {
        fprintf(stderr, "could not load file 'selectBG.png'\n");
        return;
    }

    if(!playerMarkers->loadFromFile("rec/markers.png")) {
        fprintf(stderr, "could not load file 'markers.png'\n");
        return;
    }

    if(!buffer.loadFromFile("rec/beep.wav"))
        fprintf(stderr, "could not load file 'beep.wav'\n");
    beep.setBuffer(buffer);
    beep.setVolume(60);

    bg.setTexture(*bgT);

    p1Marker.setTexture(*playerMarkers);
    p1Marker.setTextureRect(sf::IntRect(0, 0, 183, 102));
    p2Marker.setTexture(*playerMarkers);
    p2Marker.setTextureRect(sf::IntRect(74, 27, 183, 102));
    p2Marker.setOrigin(183, 102);

    if(!font.loadFromFile("rec/RevoPop.ttf")) {
        fprintf(stderr, "could not load font\n");
    }

    text.setFont(font);
    text.setString("Locked In!");
    text.setCharacterSize(32);
    text.setColor(sf::Color(32, 32, 32));

    text.setPosition(556, 344);

	printf("GCharacterSelectState Init\n");
}

void GCharacterSelectState::Cleanup()
{
	printf("GCharacterSelectState Cleanup\n");
}

void GCharacterSelectState::Pause()
{
	printf("GCharacterSelectState Pause\n");
}

void GCharacterSelectState::Resume()
{
	printf("GCharacterSelectState Resume\n");
}

void GCharacterSelectState::HandleEvents(Engine *game)
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
                if(ev.key.code == sf::Keyboard::Escape) {
                    game->ChangeState(GMenuState::Instance());
                }

                else if(ev.key.code == sf::Keyboard::Right && !p2LockIn) {
                    p2Index = (p2Index + 1) % 3;
                    beep.play();
                }

                else if(ev.key.code == sf::Keyboard::Left && !p2LockIn) {
                    p2Index--;
                    beep.play();
                    if(p2Index < 0) p2Index = 2;
                }

                else if(ev.key.code == sf::Keyboard::D && !p1LockIn) {
                    p1Index = (p1Index + 1) % 3;
                    beep.play();
                }

                else if(ev.key.code == sf::Keyboard::A && !p1LockIn) {
                    p1Index--;
                    beep.play();
                    if(p1Index < 0) p1Index = 2;
                }

                else if(ev.key.code == sf::Keyboard::LControl) {
                    if(p1LockIn && p2LockIn) {
                        GPlayState::Instance()->SetP1Type(p1Index);
                        GPlayState::Instance()->SetP2Type(p2Index);
                        GPlayState::Instance()->music = music;
                        game->ChangeState(GPlayState::Instance());
                    }
                    p1LockIn = true;
                }

                else if(ev.key.code == sf::Keyboard::LShift)
                    p1LockIn = false;

                break;

            case sf::Event::MouseButtonPressed:
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    if(p1LockIn && p2LockIn) {
                        GPlayState::Instance()->SetP1Type(p1Index);
                        GPlayState::Instance()->SetP2Type(p2Index);
                        GPlayState::Instance()->music = music;
                        game->PushState(GPlayState::Instance());
                    }
                    p2LockIn = true;
                }

                else if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    p2LockIn = false;

                break;

            default: break;
        }
    }
}

void GCharacterSelectState::Update(Engine *game) 
{
    music->update();
}

void GCharacterSelectState::Draw(Engine *game) 
{
    game->window.draw(bg);
    
    p1Marker.setPosition((p1Index * 360) + 146, 87);
    p2Marker.setPosition((p2Index * 360) + 416, 634);

    if(p1LockIn) {
        sf::RectangleShape r1(sf::Vector2f(240.f, 2.f));
        sf::RectangleShape r2(sf::Vector2f(2.f, 520.f));

        r1.setFillColor(sf::Color(32, 24, 166));
        r2.setFillColor(sf::Color(32, 24, 166));

        r1.setPosition((p1Index * 360) + 170, 89);
        r2.setPosition((p1Index * 360) + 149, 110);

        game->window.draw(r1);
        game->window.draw(r2);
        
    }
    if(p2LockIn) {
        sf::RectangleShape r1(sf::Vector2f(240.f, 2.f));
        sf::RectangleShape r2(sf::Vector2f(2.f, 520.f));

        r1.setFillColor(sf::Color(234, 17, 17));
        r2.setFillColor(sf::Color(234, 17, 17));

        r1.setPosition((p2Index * 360) + 150, 629);
        r2.setPosition((p2Index * 360) + 409, 90);

        game->window.draw(r1);
        game->window.draw(r2);
    }

    if(p1LockIn && p2LockIn) {
        sf::RectangleShape r1(sf::Vector2f(1280.f, 50.f));
        r1.setFillColor(sf::Color(200, 200, 200));
        r1.setOutlineColor(sf::Color(32, 32, 32));
        r1.setOutlineThickness(4);
        r1.setPosition(0, 335);
        game->window.draw(r1);
        game->window.draw(text);
    }

    game->window.draw(p1Marker);
    game->window.draw(p2Marker);
}
