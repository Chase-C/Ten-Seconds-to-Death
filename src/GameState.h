#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Engine.h"
#include "GMusic.h"
#include "InputManager.h"

class GameState
{
public:
	virtual void Init() = 0;
	virtual void Cleanup() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents(Engine* game) = 0;
	virtual void Update(Engine* game) = 0;
	virtual void Draw(Engine* game) = 0;

    static GMusic *music;
    static InputManager *inputManager;

	void ChangeState(Engine* game, GameState* state) {
		game->ChangeState(state);
	}

protected:
	GameState() { }
};

#endif

