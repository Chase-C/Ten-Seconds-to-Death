#include <stdio.h>
#include <string.h>
#include <SFML/Graphics.hpp>

#include "Engine.h"
#include "GameState.h"
#include "GPlayState.h"
#include "GMenuState.h"

GMenuState GMenuState::g_MenuState;

// pass these to the menu items
void Play(Engine *game);
void HighScores(Engine *game);
void Quit(Engine *game);

void GMenuState::Init()
{
	items = new GMenuItem*[3];
	menuIndex = 0;

	for(int i = 0; i < 3; i++)
		items[i] = new GMenuItem();

	items[0]->Init("Play!", 100, 500, Play);
	items[1]->Init("High Scores!", 250, 500, HighScores);
	items[2]->Init("Exit :(", 450, 500, Quit);

	items[menuIndex]->Select();

	printf("GMenuState Init\n");
}

void GMenuState::Cleanup()
{
	for(int i = 0; i < 3; i++) {
		items[i]->Cleanup();
		delete items[i];
	}
	delete items;

	printf("GMenuState Cleanup\n");
}

void GMenuState::Pause()
{
	printf("GMenuState Pause\n");
}

void GMenuState::Resume()
{
	printf("GMenuState Resume\n");
}

void GMenuState::HandleEvents(Engine *game)
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
                    game->window.close();
                    game->Quit();
                }

                else if(ev.key.code == sf::Keyboard::Right)
                    NextItem();

                else if(ev.key.code == sf::Keyboard::Left)
                    PrevItem();

                else if(ev.key.code == sf::Keyboard::Return || ev.key.code == sf::Keyboard::Space)
                    items[menuIndex]->Activate(game);

                break;

            default: break;
        }
    }
}

void GMenuState::Update(Engine *game) 
{

}

void GMenuState::NextItem()
{
    items[menuIndex]->DeSelect();

    menuIndex++;

    if(menuIndex > 2)
        menuIndex = 0;

    items[menuIndex]->Select();
}

void GMenuState::PrevItem()
{
    items[menuIndex]->DeSelect();

    menuIndex--;

    if(menuIndex < 0)
        menuIndex = 2;

    items[menuIndex]->Select();
}

void GMenuState::Draw(Engine *game) 
{
    // draw menu items
    for(int i = 0; i < 3; i++)
        items[i]->Draw(game);

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//
// Start of the GMenuItem class
//
//-------------------------------------------------------------------
//-------------------------------------------------------------------

void GMenuItem::Init(char *t, int x, int y, void (*e)(Engine *game))
{
    callback = e;

    if(!font.loadFromFile("RevoPop.ttf")) {
        fprintf(stderr, "could not load font\n");
    }

    text.setFont(font);
    text.setString(t);
    text.setCharacterSize(14);
    text.setColor(sf::Color(0, 0, 0));

    text.setPosition(x, y);
}

void GMenuItem::Cleanup()
{
    printf("Items cleaned up");
}

void GMenuItem::Select()
{
    text.setColor(sf::Color(64, 64, 64));
    text.move(0.f, -10.f);
}

void GMenuItem::DeSelect()
{
	text.setColor(sf::Color(0, 0, 0));
    text.move(0.f, 10.f);
}

void GMenuItem::Activate(Engine *game)
{
	callback(game);
}

void GMenuItem::Draw(Engine *game)
{
    game->window.draw(text);
}


// the callback functions for each menu item

void Play(Engine *game)
{
	game->PushState(GPlayState::Instance());
}

void HighScores(Engine *game)
{

}

void Quit(Engine *game)
{
	game->Quit();
}
