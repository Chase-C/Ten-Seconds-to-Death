#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SFML/Graphics.hpp>

#include "Engine.h"
#include "GameState.h"
#include "GMenuState.h"
#include "GCharacterSelectState.h"

GMenuState GMenuState::g_MenuState;

// pass these to the menu items
void Play(Engine *game);
void Instructions(Engine *game);
void Quit(Engine *game);

void GMenuState::Init()
{
    srand(time(NULL));
    bg = new sf::Texture();
    if(!bg->loadFromFile("rec/title.png"))
        fprintf(stderr, "could not find file 'title.png'\n");

    if(!buffer.loadFromFile("rec/beep.wav"))
        fprintf(stderr, "could not load file 'beep.wav'\n");
    beep.setBuffer(buffer);
    beep.setVolume(60);

    bgSprite.setTexture(*bg);
    bgSprite.setPosition(sf::Vector2f(0.f, 0.f));

	items = new GMenuItem*[3];
	menuIndex = 0;

	for(int i = 0; i < 3; i++)
		items[i] = new GMenuItem();

	items[0]->Init("Play!", 150, 300, Play);
	items[1]->Init("Instructions", 300, 400, Instructions);
	items[2]->Init("Exit :(", 450, 500, Quit);

	items[menuIndex]->Select();

    instructions = false;

    if(!font.loadFromFile("rec/RevoPop.ttf")) {
        fprintf(stderr, "could not load font\n");
    }

    text.setFont(font);
    text.setString("Player 1 Controls           Player 2 Controls\
                    \n W/A/S/D                     Up/Down/Left/Right\
                    \n LControl - Attack          LClick - Attack\
                    \n LShift - Dash                 RClick - Dash\
                    \n\nYour character's ultimate move is activated automatically\
                    \nevery ten seconds. Press the attack button while performing\
                    \nit to start a combo. You will be unable to dash or combo if\
                    \nyou don't have enough stamina. To finish off the other\
                    \nplayer, you need to hit them with you ultimate.");
    text.setCharacterSize(14);
    text.setColor(sf::Color(0, 0, 0));
    text.setPosition(280, 100);

	printf("GMenuState Init\n");
}

void GMenuState::Cleanup()
{
	for(int i = 0; i < 3; i++) {
		items[i]->Cleanup();
		delete items[i];
	}
	delete items;
    delete bg;

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
                    if(instructions) {
                        instructions = false;
                    } else {
                        game->window.close();
                        game->Quit();
                    }
                }

                else if(ev.key.code == sf::Keyboard::Right || ev.key.code == sf::Keyboard::Down) {
                    NextItem();
                    beep.play();
                }

                else if(ev.key.code == sf::Keyboard::Left || ev.key.code == sf::Keyboard::Up) {
                    PrevItem();
                    beep.play();
                }

                else if(ev.key.code == sf::Keyboard::Return || ev.key.code == sf::Keyboard::Space) {
                    if(menuIndex == 0) GCharacterSelectState::Instance()->music = music;
                    if(menuIndex == 1) instructions = !instructions;
                    items[menuIndex]->Activate(game);
                }

                break;

            default: break;
        }
    }
}

void GMenuState::Update(Engine *game) 
{
    music->update();
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
    
    game->window.draw(bgSprite);
    for(int i = 0; i < 3; i++)
        items[i]->Draw(game);

    if(instructions)
        game->window.draw(text);

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

    if(!font.loadFromFile("rec/RevoPop.ttf")) {
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
    text.setColor(sf::Color(64, 128, 255));
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
	game->ChangeState(GCharacterSelectState::Instance());
}

void Instructions(Engine *game)
{
}

void Quit(Engine *game)
{
	game->Quit();
}
