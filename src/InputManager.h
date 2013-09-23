#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SFML/Graphics.hpp>
#include "Engine.h"
#include "Character.h"

class InputManager
{
public:
	InputManager();
    void addPlayer(Character *p);

    unsigned int convert(unsigned int code, sf::Event::EventType type, unsigned int id = 0, int pos = 0);

    bool isUp1(unsigned int code);
    bool isDown1(unsigned int code);
    bool isLeft1(unsigned int code);
    bool isRight1(unsigned int code);
    bool isAttack1(unsigned int code);
    bool isDash1(unsigned int code);

    bool isUp2(unsigned int code);
    bool isDown2(unsigned int code);
    bool isLeft2(unsigned int code);
    bool isRight2(unsigned int code);
    bool isAttack2(unsigned int code);
    bool isDash2(unsigned int code);

    void setUp1(unsigned int code);
    void setDown1(unsigned int code);
    void setLeft1(unsigned int code);
    void setRight1(unsigned int code);
    void setAttack1(unsigned int code);
    void setDash1(unsigned int code);

    void setUp2(unsigned int code);
    void setDown2(unsigned int code);
    void setLeft2(unsigned int code);
    void setRight2(unsigned int code);
    void setAttack2(unsigned int code);
    void setDash2(unsigned int code);

	void keyUpdate(unsigned int code, bool state);
	void mouseUpdate(unsigned int code, bool state);
	void joystickButtonUpdate(unsigned int id, unsigned int code, bool state);
	void joystickMoveUpdate(unsigned int id, unsigned int code, int pos);

	void playStateUpdate();

    char *SFKeyToString(unsigned int code);

    char *up1String();
    char *down1String();
    char *left1String();
    char *right1String();
    char *attack1String();
    char *dash1String();

    char *up2String();
    char *down2String();
    char *left2String();
    char *right2String();
    char *attack2String();
    char *dash2String();

    void loadKeys();
    void saveKeys();

private:
    void init();
    void update(unsigned int code, bool state);

	Character *player1;
	Character *player2;
    bool p1, p2;

	bool	up1, down1,
			left1, right1,
            attack1, dash1,
	        up2, down2,
			left2, right2,
            attack2, dash2,
			esc;

	unsigned int 	upKey1, downKey1,
			        leftKey1, rightKey1,
                    attackKey1, dashKey1,
	                upKey2, downKey2,
			        leftKey2, rightKey2,
                    attackKey2, dashKey2;
};

#endif
