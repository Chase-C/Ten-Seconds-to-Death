#include "Engine.h"
#include "GInitState.h"

int main()
{
	Engine game;

	// initialize the engine
	if(!game.Init("Platfomer v0.01")) {
		return 1;
	}

	// load the intro
	game.ChangeState(GInitState::Instance());

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
