#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SFML/Graphics.hpp>
#include "Engine.h"
#include "Character.h"

class InputManager
{
public:
	InputManager() { };
	InputManager(Character *c1);
	InputManager(Character *c1, Character *c2);

	void eventUpdate(int keyCode, bool state);
	void Update();

private:
    void init();
    void changeState(int key, bool state);

	Character *player1;
	Character *player2;
    bool p2;

	bool	up1, down1,
			left1, right1,
            attack1, dash1,
	        up2, down2,
			left2, right2,
            attack2, dash2,
			esc;

	int 	upKey1, downKey1,
			leftKey1, rightKey1,
            attackKey1, dashKey1,
	        upKey2, downKey2,
			leftKey2, rightKey2,
            attackKey2, dashKey2;
};

#endif
