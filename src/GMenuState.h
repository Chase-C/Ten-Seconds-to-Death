#ifndef GMENUSTATE_H
#define GMENUSTATE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "GameState.h"

class GMenuItem
{
public:
	void Init(char *t, int x, int y, bool vert);
	void Init(char *t, int x, int y, char *ot, int ox, int oy, bool vert);
	void Cleanup();

	void Select();
	void DeSelect();

	void Draw(Engine *game);

    sf::Text otherText;

private:
    bool verticle;
    sf::Font font;
    sf::Color color;
    sf::Text text;

    bool other;
};

class GMenuState : public GameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(Engine *game);
	void Update(Engine *game);
	void Draw(Engine *game);

	static GMenuState *Instance() {
		return &g_MenuState;
	}

protected:
	GMenuState() { }

private:
	static GMenuState g_MenuState;

    void resetOptionString();

	GMenuItem items[4];
    GMenuItem optionsMenu[12];
	int menuIndex;
    int optionIndex;

	void NextItem();
	void PrevItem();

    bool instructions,
         options,
         newKey;

    sf::Font font;
    sf::Color color;
    sf::Text text;

    sf::Texture *bg;
    sf::Sprite bgSprite;

    sf::SoundBuffer buffer;
    sf::Sound beep;
};

#endif
