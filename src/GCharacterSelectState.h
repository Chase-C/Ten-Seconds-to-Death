#ifndef GSELECTIONSTATE_H
#define GSELECTIONSTATE_H

#include <SFML/Graphics.hpp>

#include "GameState.h"

class GCharacterSelectState : public GameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(Engine *game);
	void Update(Engine *game);
	void Draw(Engine *game);

	static GCharacterSelectState *Instance() {
		return &g_CharacterSelectState;
	}

protected:
	GCharacterSelectState() { }

private:
	static GCharacterSelectState g_CharacterSelectState;

	int p1Index;
	int p2Index;

    bool p1LockIn;
    bool p2LockIn;

    sf::Font font;
    sf::Color color;
    sf::Text text;

    sf::Texture *playerMarkers;
    sf::Sprite p1Marker, p2Marker;

    sf::Texture *bgT;
    sf::Sprite bg;

    sf::SoundBuffer buffer;
    sf::Sound beep;
};

#endif
