#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SFML/Graphics.hpp>

#include "Engine.h"
#include "GameState.h"
#include "GMenuState.h"
#include "GCharacterSelectState.h"

GMusic *GameState::music = new GMusic();
InputManager *GameState::inputManager = new InputManager();

GMenuState GMenuState::g_MenuState;

void GMenuState::Init()
{
    srand(time(NULL));

    music->play();

    bg = new sf::Texture();
    if(!bg->loadFromFile("rec/title.png"))
        fprintf(stderr, "could not find file 'title.png'\n");

    if(!buffer.loadFromFile("rec/beep.wav"))
        fprintf(stderr, "could not load file 'beep.wav'\n");
    beep.setBuffer(buffer);
    beep.setVolume(60);

    bgSprite.setTexture(*bg);
    bgSprite.setPosition(sf::Vector2f(0.f, 0.f));

	menuIndex = 0;
	items[0].Init("Play!", 100, 300, true);
	items[1].Init("Instructions", 225, 375, true);
	items[2].Init("Options", 350, 450, true);
	items[3].Init("Exit :(", 475, 525, true);

    optionIndex = 0;
    char *strings[6] = {"Up", "Down", "Left", "Right", "Attack", "Dash"};
    char *keys[12] = {
        inputManager->up1String(),
        inputManager->down1String(),
        inputManager->left1String(),
        inputManager->right1String(),
        inputManager->attack1String(),
        inputManager->dash1String(),
        inputManager->up2String(),
        inputManager->down2String(),
        inputManager->left2String(),
        inputManager->right2String(),
        inputManager->attack2String(),
        inputManager->dash2String()
    };
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 6; j++) {
            int k = (i * 6) + j;
            char *str;
            sprintf(str, "Player %i %s", i + 1, strings[j]);
            optionsMenu[k].Init(str, 200, 140 + (k * 36) + (i * 16), keys[k], 400, 140 + (k * 36) + (i * 16), false);
        }
    }

    delete [] strings;
    delete [] keys;

    items[menuIndex].Select();
    optionsMenu[optionIndex].Select();

    instructions = false;
    options = false;
    newKey = false;

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
	for(int i = 0; i < 3; i++)
		items[i].Cleanup();

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
        int button = 0, pos = 0, id = 0;

        switch(ev.type) {
            case sf::Event::Closed:
                game->window.close();
                game->Quit();
                break;

            case sf::Event::KeyPressed:
                button = ev.key.code;

                if(ev.key.code == sf::Keyboard::Escape) {
                    if(newKey) {
                        newKey = false;
                        resetOptionString();
                    } else if(instructions) {
                        instructions = false;
                    } else if(options) {
                        options = false;
                        instructions = false;
                    } else {
                        game->window.close();
                        game->Quit();
                    }
                    continue;
                } else if(!newKey) {
                    if((!options && ev.key.code == sf::Keyboard::Right) || ev.key.code == sf::Keyboard::Down) {
                        NextItem();
                        continue;
                    } else if((!options && ev.key.code == sf::Keyboard::Left) || ev.key.code == sf::Keyboard::Up) {
                        PrevItem();
                        continue;
                    } else if(ev.key.code == sf::Keyboard::Return || ev.key.code == sf::Keyboard::Space) {
                        if(!options) {
                            if(menuIndex == 0) game->ChangeState(GCharacterSelectState::Instance());
                            else if(menuIndex == 1) instructions = !instructions;
                            else if(menuIndex == 2) options = !options;
                            else if(menuIndex == 3) game->Quit();
                        } else {
                            newKey = true;
                            optionsMenu[optionIndex].otherText.setString("<Enter New Key>");
                        }
                        continue;
                    }
                }

                break;

            case sf::Event::MouseButtonPressed:
                button = ev.mouseButton.button;
                break;
            case sf::Event::JoystickButtonPressed:
                button = ev.joystickButton.button;
                id = ev.joystickButton.joystickId;
                break;
            case sf::Event::JoystickMoved:
                button = ev.joystickMove.axis;
                id = ev.joystickMove.joystickId;
                pos = ev.joystickMove.position;
                if(abs(pos) < 5) continue;
                break;
            default: continue;
        }

        int code = inputManager->convert(button, ev.type, id, pos);
        printf("%i - %i\n", button, code);

        if(newKey) {
            switch(optionIndex) {
                case 0: inputManager->setUp1(code); break;
                case 1: inputManager->setDown1(code); break;
                case 2: inputManager->setLeft1(code); break;
                case 3: inputManager->setRight1(code); break;
                case 4: inputManager->setAttack1(code); break;
                case 5: inputManager->setDash1(code); break;
                case 6: inputManager->setUp2(code); break;
                case 7: inputManager->setDown2(code); break;
                case 8: inputManager->setLeft2(code); break;
                case 9: inputManager->setRight2(code); break;
                case 10: inputManager->setAttack2(code); break;
                case 11: inputManager->setDash2(code); break;
            }
            resetOptionString();
            newKey = false;
        } else {
            if((!options && inputManager->isRight1(code)) || inputManager->isDown1(code))
                NextItem();

            else if((!options && inputManager->isLeft1(code)) || inputManager->isUp1(code))
                PrevItem();

            else if(inputManager->isAttack1(code)) {
                if(!options) {
                    if(menuIndex == 0) game->ChangeState(GCharacterSelectState::Instance());
                    else if(menuIndex == 1) instructions = !instructions;
                    else if(menuIndex == 2) options = !options;
                    else if(menuIndex == 3) game->Quit();
                } else {
                    newKey = true;
                    optionsMenu[optionIndex].otherText.setString("<Enter New Key>");
                }
            }
        }
    }
}

void GMenuState::resetOptionString()
{
    switch(optionIndex) {
        case 0: optionsMenu[optionIndex].otherText.setString(inputManager->up1String()); break;
        case 1: optionsMenu[optionIndex].otherText.setString(inputManager->down1String()); break;
        case 2: optionsMenu[optionIndex].otherText.setString(inputManager->left1String()); break;
        case 3: optionsMenu[optionIndex].otherText.setString(inputManager->right1String()); break;
        case 4: optionsMenu[optionIndex].otherText.setString(inputManager->attack1String()); break;
        case 5: optionsMenu[optionIndex].otherText.setString(inputManager->dash1String()); break;
        case 6: optionsMenu[optionIndex].otherText.setString(inputManager->up2String()); break;
        case 7: optionsMenu[optionIndex].otherText.setString(inputManager->down2String()); break;
        case 8: optionsMenu[optionIndex].otherText.setString(inputManager->left2String()); break;
        case 9: optionsMenu[optionIndex].otherText.setString(inputManager->right2String()); break;
        case 10: optionsMenu[optionIndex].otherText.setString(inputManager->attack2String()); break;
        case 11: optionsMenu[optionIndex].otherText.setString(inputManager->dash2String()); break;
    }
}

void GMenuState::Update(Engine *game) 
{
    music->update();
}

void GMenuState::NextItem()
{
    beep.play();
    if(!options) {
        items[menuIndex].DeSelect();
        menuIndex++;

        if(menuIndex > 3)
            menuIndex = 0;

        items[menuIndex].Select();
    } else {
        optionsMenu[optionIndex].DeSelect();
        optionIndex++;

        if(optionIndex > 11)
            optionIndex = 0;

        optionsMenu[optionIndex].Select();
    }
}

void GMenuState::PrevItem()
{
    beep.play();
    if(!options) {
        items[menuIndex].DeSelect();
        menuIndex--;

        if(menuIndex < 0)
            menuIndex = 3;

        items[menuIndex].Select();
    } else {
        optionsMenu[optionIndex].DeSelect();
        optionIndex--;

        if(optionIndex < 0)
            optionIndex = 11;

        optionsMenu[optionIndex].Select();
    }
}

void GMenuState::Draw(Engine *game) 
{
    // draw menu items

    game->window.draw(bgSprite);
    if(!options) {
        for(int i = 0; i < 4; i++)
            items[i].Draw(game);

        if(instructions)
            game->window.draw(text);
    } else {
        for(int i = 0; i < 12; i++)
            optionsMenu[i].Draw(game);
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//
// Start of the GMenuItem class
//
//-------------------------------------------------------------------
//-------------------------------------------------------------------

void GMenuItem::Init(char *t, int x, int y, bool vert)
{
    if(!font.loadFromFile("rec/RevoPop.ttf")) {
        fprintf(stderr, "could not load font\n");
    }

    text.setFont(font);
    text.setString(t);
    text.setCharacterSize(14);
    text.setColor(sf::Color(0, 0, 0));

    text.setPosition(x, y);
    verticle = vert;
    other = false;
}

void GMenuItem::Init(char *t, int x, int y, char *ot, int ox, int oy, bool vert)
{
    Init(t, x, y, vert);
    otherText.setFont(font);
    otherText.setString(ot);
    otherText.setCharacterSize(14);
    otherText.setColor(sf::Color(0, 0, 0));

    otherText.setPosition(ox, oy);
    other = true;
}

void GMenuItem::Cleanup()
{
    printf("Items cleaned up");
}

void GMenuItem::Select()
{
    text.setColor(sf::Color(64, 128, 255));
    if(verticle)
        text.move(0.f, -10.f);
    else
        text.move(10.f, 0.f);
}

void GMenuItem::DeSelect()
{
	text.setColor(sf::Color(0, 0, 0));
    if(verticle)
        text.move(0.f, 10.f);
    else
        text.move(-10.f, 0.f);
}

void GMenuItem::Draw(Engine *game)
{
    game->window.draw(text);

    if(other)
        game->window.draw(otherText);
}
