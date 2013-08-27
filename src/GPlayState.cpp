#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <chipmunk/chipmunk.h>
#include <cmath>

#include "Engine.h"
#include "GMenuState.h"
#include "GPlayState.h"

GPlayState GPlayState::g_PlayState;

GPlayState::GPlayState()
{
    player1 = NULL;
    player2 = NULL;
}

void GPlayState::Init()
{
    srand(time(NULL));
    stepSize = 1.0 / 60.0;
    space = cpSpaceNew();
    cpSpaceSetIterations(space, 6);

	arena = Arena(space);

    if(!player1)
        player1 = new Fighter();
    if(!player2)
        player2 = new Fighter();

    player1->Init(space, 250, 400, Character::RIGHT, sf::Color(32, 32, 200));
    player2->Init(space, 1050, 400, Character::LEFT, sf::Color(200, 32, 32));

    player1->setOponent(player2);
    player2->setOponent(player1);

	inputManager = InputManager(player1, player2);
    hud = HUD(player1, player2);

    if(!font.loadFromFile("rec/RevoPop.ttf")) {
        fprintf(stderr, "could not load font\n");
    }

    p1ScoreT.setFont(font);
    p1ScoreT.setString("0");
    p1ScoreT.setCharacterSize(32);
    p1ScoreT.setColor(sf::Color(32, 32, 32));
    p1ScoreT.setPosition(556, 344);

    p2ScoreT.setFont(font);
    p2ScoreT.setString("0");
    p2ScoreT.setCharacterSize(32);
    p2ScoreT.setColor(sf::Color(32, 32, 32));
    p2ScoreT.setPosition(556, 344);

    winner.setFont(font);
    winner.setCharacterSize(32);
    winner.setColor(sf::Color(32, 32, 32));
    winner.setPosition(480, 344);

    reseting = true;
    resetTime.restart();
    winner.setString("Beginning Round 1");
    round = 1;
    p1Score = 0;
    p2Score = 0;

    int ultPlayer = rand() % 2;
    ultClock.restart();

	printf("GPlayState Init\n");
}

void GPlayState::SetP1Type(int type)
{
    switch(type) {
        case 0:
            player1 = new Ninja();
            break;
        case 1:
            player1 = new Shooter();
            break;
        case 2:
        default:
            player1 = new Fighter();
            break;
    }
}

void GPlayState::SetP2Type(int type)
{
    switch(type) {
        case 0:
            player2 = new Ninja();
            break;
        case 1:
            player2 = new Shooter();
            break;
        case 2:
        default:
            player2 = new Fighter();
            break;
    }
}

void GPlayState::Cleanup()
{
	printf("GPlayState Cleanup\n");
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
                
            case sf::Event::MouseButtonPressed:
                inputManager.mouseUpdate();
                break;

            case sf::Event::MouseButtonReleased:
                inputManager.mouseUpdate();
                break;
                
            default: break;
        }
    }
}

void GPlayState::Update(Engine* game)
{
    if(player1->isDead() && !reseting) {
        reseting = true;
        p1Score++;
        std::string s = "Player 1 Wins Round ";
        if(round == 1) s += "1";
        else if(round == 2) s += "2";
        else s += "3";

        winner.setString(s);
        resetTime.restart();
    } else if(player2->isDead() && !reseting) {
        reseting = true;
        p2Score++;
        std::string s = "Player 2 Wins Round ";
        if(round == 1) s += "1";
        else if(round == 2) s += "2";
        else s += "3";

        winner.setString(s);
        resetTime.restart();
    } else if(reseting) {
        if(resetTime.getElapsedTime() > sf::seconds(5)) {
            reseting = false;
            player1->ReInit(250, 400, Character::RIGHT);
            player2->ReInit(1050, 400, Character::LEFT);
            ultClock.restart();
        } else if(resetTime.getElapsedTime() > sf::seconds(4))
            winner.setString("Starting In... 1");
        else if(resetTime.getElapsedTime() > sf::seconds(3))
            winner.setString("Starting In... 2");
        else if(resetTime.getElapsedTime() > sf::seconds(2)) {
            winner.setString("Starting In... 3");
            winner.setPosition(512, 344);
            if(p1Score >= 2 || p2Score >= 2)
                game->ChangeState(GMenuState::Instance());
        }
    } else {
        inputManager.Update();

        if(ultClock.getElapsedTime() > sf::seconds(5.0)) {
            ultClock.restart();
            ultPlayer = (ultPlayer + 1) % 2;

            if(ultPlayer) player2->ult();
            else player1->ult();
        }

        player1->update();
        player2->update();

        float step = stepSize / 3.0;
        for(int i = 0; i < 3; i++) {
            player1->physUpdate();
            player2->physUpdate();

            cpSpaceStep(space, step);
        }
    }
}

void GPlayState::Quit(Engine* game)
{
    game->ChangeState(GMenuState::Instance());
}

void GPlayState::Draw(Engine* game)
{
    sf::Transform trans = sf::Transform::Identity;

    arena.drawArena(&game->window, trans);
    hud.drawHUD(&game->window, trans);

    player1->draw(&game->window, trans);
    player2->draw(&game->window, trans);

    if(player1->type == Character::SHOOTER)
        ((Shooter*)player1)->drawBullets(&game->window, trans);
    if(player2->type == Character::SHOOTER)
        ((Shooter*)player2)->drawBullets(&game->window, trans);

    if(reseting) {
        sf::RectangleShape r1(sf::Vector2f(1280.f, 50.f));
        r1.setFillColor(sf::Color(200, 200, 200));
        r1.setOutlineColor(sf::Color(32, 32, 32));
        r1.setOutlineThickness(4);
        r1.setPosition(0, 335);
        game->window.draw(r1);
        game->window.draw(winner);
    }
}
