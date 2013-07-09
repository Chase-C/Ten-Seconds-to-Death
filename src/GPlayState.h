#ifndef GPLAYSTATE_H
#define GPLAYSTATE_H

#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <chipmunk/chipmunk.h>

#include "GameState.h"
#include "Terrain.h"
#include "InputManager.h"
#include "AIController.h"
#include "Character.h"
#include "Camera.h"

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

    Terrain *getTerrain();

	static GPlayState* Instance() {
		return &g_PlayState;
	}

protected:
	GPlayState() { }

private:
	static GPlayState g_PlayState;

    double stepSize;
    cpSpace *space;

	InputManager inputManager;

	Terrain terrain;
    Camera cam;

    Character *player;
    std::vector<Character*> characters;
    std::vector<AIController*> AIs;
};

#endif
