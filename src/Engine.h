#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>

using namespace std;

class GameState;

class Engine
{
public:

	bool Init(const char* title, int width=800, int height=600);
	void Cleanup();

	void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() {
		return running;
	}
	void Quit() {
		running = false;
	}

    sf::RenderWindow window;

private:
	// the stack of states
	vector<GameState*> states;
	bool running;

    sf::Clock clock;
    sf::Time frameTimeLimit;
};

#endif
