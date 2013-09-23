#include "Engine.h"
#include "GMenuState.h"
#include "GMusic.h"
#include "InputManager.h"

int main()
{
	Engine game;

	// initialize the engine
	if(!game.Init("10 Seconds")) {
		return 1;
	}

    game.window.setJoystickThreshold(10);
	game.ChangeState(GMenuState::Instance());

	// main loop
	while(game.Running()) {
		game.HandleEvents();
		game.Update();
		if(game.Running())
			game.Draw();
	}

	// cleanup the engine
	game.Cleanup();

	return 0;
}
