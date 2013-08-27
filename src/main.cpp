#include "Engine.h"
#include "GMenuState.h"

int main()
{
	Engine game;

	// initialize the engine
	if(!game.Init("10 Seconds")) {
		return 1;
	}

	// load the intro
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
