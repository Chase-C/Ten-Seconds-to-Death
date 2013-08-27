#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SFML/Graphics.hpp>
#include "Engine.h"
#include "Character.h"

class InputManager
{
public:
	InputManager() { };
	InputManager(Character *c1, Character *c2);

	void CheckKeyPress(int keyCode);
	void CheckKeyRelease(int keyCode);

    void mouseUpdate();

	void Update();

private:
	Character *player1;
	Character *player2;

	bool	up, down,
			left, right,
            w, a, s, d,
            shift, control,
			esc;

    bool rClick, lClick;
};

#endif
