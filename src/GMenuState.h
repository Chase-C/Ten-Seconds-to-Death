#ifndef GMENUSTATE_H
#define GMENUSTATE_H

#include <SFML/Graphics.hpp>

#include "GameState.h"

class GMenuItem;

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

	GMenuItem **items;
	int menuIndex;

	void NextItem();
	void PrevItem();

    bool instructions;

    sf::Font font;
    sf::Color color;
    sf::Text text;

    sf::Texture *bg;
    sf::Sprite bgSprite;
};


typedef void (*CallbackFunc)(Engine*);

class GMenuItem
{
public:
	void Init(char *t, int X, int Y, void (*e)(Engine*));
	void Cleanup();

	void Select();
	void DeSelect();

	void Activate(Engine *game);

	void Draw(Engine *game);

private:
	int x, y;

    sf::Font font;
    sf::Color color;
    sf::Text text;

	CallbackFunc callback;
};

#endif
