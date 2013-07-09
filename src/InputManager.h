#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SFML/Graphics.hpp>
#include "Engine.h"
#include "Character.h"

class InputManager
{
public:
	InputManager() { };
	InputManager(Character* c);

	void CheckKeyPress(int keyCode);
	void CheckKeyRelease(int keyCode);

    void mousePress(Engine *game);

	void Update();

private:
	Character* character;

	bool	up, down,
			left, right,
			pause;
					
    int     relX, relY;
	bool	lClick, rClick;

	int 	upKeys[2], downKeys[2],
			leftKeys[2], rightKeys[2],
			pauseKeys[2];
};

#endif
