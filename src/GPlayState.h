#ifndef GPLAYSTATE_H
#define GPLAYSTATE_H

#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <chipmunk/chipmunk.h>

#include "GameState.h"
#include "Arena.h"
#include "HUD.h"
#include "InputManager.h"
#include "Character.h"
#include "Fighter.h"
#include "Shooter.h"
#include "Bullet.h"
#include "Ninja.h"

class GPlayState : public GameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void Quit(Engine* game);

	void HandleEvents(Engine* game);
	void Update(Engine* game);
	void Draw(Engine* game);

    void SetP1Type(int type);
    void SetP2Type(int type);

	static GPlayState* Instance() {
		return &g_PlayState;
	}

protected:
    GPlayState();

private:
	static GPlayState g_PlayState;

    void checkBulletCollision(Bullet *b, Character *c);

    void reset();
    bool reseting;
    sf::Clock resetTime;

    int round;
    int p1Score;
    int p2Score;

    double stepSize;
    cpSpace *space;

	InputManager inputManager;

	Arena arena;
    HUD hud;

    Character *player1;
    Character *player2;

    sf::Clock ultClock;
    int ultPlayer;

    sf::Font font;
    sf::Color color;
    sf::Text p1ScoreT;
    sf::Text p2ScoreT;
    sf::Text winner;
};

#endif
