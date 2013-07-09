#ifndef GINITSTATE_H
#define GINITSTATE_H

#include <SFML/graphics.hpp>
#include <SFML/System.hpp>
#include "GameState.h"

class GInitState : public GameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(Engine* game);
	void Update(Engine* game);
	void Draw(Engine* game);

	static GInitState* Instance() {
		return &g_InitState;
	}

protected:
	GInitState() { }

private:
	static GInitState g_InitState;

    sf::Texture bg;
    sf::Sprite sprite;
	double alpha;

    sf::Clock clock;
    sf::Time totalTime;
    sf::Time fadeTime;
};

#endif
