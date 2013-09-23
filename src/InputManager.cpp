#include <SFML/Graphics.hpp>
#include <cmath>

#include "InputManager.h"
#include "Character.h"

InputManager::InputManager()
{
    init();

    p1 = false;
    p2 = false;
}

void InputManager::addPlayer(Character *p)
{
    if(!p1) {
        player1 = p;
        p1 = true;
    } else if(!p2) {
        player2 = p;
        p2 = true;
    }
}

void InputManager::init()
{
	up1 = false;
	down1 = false;
	left1 = false;
	right1 = false;
    attack1 = false;
    dash1 = false;

	up2 = false;
	down2 = false;
	left2 = false;
	right2 = false;
    attack2 = false;
    dash2 = false;

	esc = false;

	upKey1 = sf::Keyboard::Up;
	downKey1 = sf::Keyboard::Down;
	leftKey1 = sf::Keyboard::Left;
	rightKey1 = sf::Keyboard::Right;
    attackKey1 = sf::Mouse::Left + 128;
    dashKey1 = sf::Mouse::Right + 128;

	upKey2 = sf::Keyboard::W;
	downKey2 = sf::Keyboard::S;
	leftKey2 = sf::Keyboard::A;
	rightKey2 = sf::Keyboard::D;
    attackKey2 = sf::Keyboard::LControl;
    dashKey2 = sf::Keyboard::LShift;
}

unsigned int InputManager::convert(unsigned int code, sf::Event::EventType type, unsigned int id, int pos)
{
    switch(type) {
        case sf::Event::MouseButtonPressed:
        case sf::Event::MouseButtonReleased:
            code += 128;
            break;
        case sf::Event::JoystickButtonPressed:
        case sf::Event::JoystickButtonReleased:
            code += 256;
            if(id == 1) code += 128;
            break;
        case sf::Event::JoystickMoved:
            code <<= 1;
            code += 256 + 64;
            if(id == 1) code += 128;

            if(pos == 0) return code + 512; // Nothing can bind to this number
            else if(pos < 0) code += 1;
        default:
            break;
    }

    return code;
}

bool InputManager::isUp1(unsigned int code) { return code == upKey1; }
bool InputManager::isDown1(unsigned int code) { return code == downKey1; }
bool InputManager::isLeft1(unsigned int code) { return code == leftKey1; }
bool InputManager::isRight1(unsigned int code) { return code == rightKey1; }
bool InputManager::isAttack1(unsigned int code) { return code == attackKey1; }
bool InputManager::isDash1(unsigned int code) { return code == dashKey1; }

bool InputManager::isUp2(unsigned int code) { return code == upKey2; }
bool InputManager::isDown2(unsigned int code) { return code == downKey2; }
bool InputManager::isLeft2(unsigned int code) { return code == leftKey2; }
bool InputManager::isRight2(unsigned int code) { return code == rightKey2; }
bool InputManager::isAttack2(unsigned int code) { return code == attackKey2; }
bool InputManager::isDash2(unsigned int code) { return code == dashKey2; }

void InputManager::setUp1(unsigned int code) { upKey1 = code; }
void InputManager::setDown1(unsigned int code) { downKey1 = code; }
void InputManager::setLeft1(unsigned int code) { leftKey1 = code; }
void InputManager::setRight1(unsigned int code) { rightKey1 = code; }
void InputManager::setAttack1(unsigned int code) { attackKey1 = code; }
void InputManager::setDash1(unsigned int code) { dashKey1 = code; }

void InputManager::setUp2(unsigned int code) { upKey2 = code; }
void InputManager::setDown2(unsigned int code) { downKey2 = code; }
void InputManager::setLeft2(unsigned int code) { leftKey2 = code; }
void InputManager::setRight2(unsigned int code) { rightKey2 = code; }
void InputManager::setAttack2(unsigned int code) { attackKey2 = code; }
void InputManager::setDash2(unsigned int code) { dashKey2 = code; }

void InputManager::keyUpdate(unsigned int code, bool state)
{
    update(code, state);
}

void InputManager::mouseUpdate(unsigned int code, bool state)
{
    update(code + 128, state);
}

void InputManager::joystickButtonUpdate(unsigned int id, unsigned int code, bool state)
{
    code += 256;
    if(id == 1) code += 128;
    update(code, state);
}

void InputManager::joystickMoveUpdate(unsigned int id, unsigned int code, int pos)
{
    code <<= 1;
    code += 256 + 64;
    if(id == 1) code += 128;

    if(pos == 0) {
        update(code, false);
        update(code + 1, false);
    } else {
        bool state = (pos > 0 ? true : false);
        update(code, state);
        update(code + 1, !state);
    }
}

void InputManager::update(unsigned int code, bool state)
{
    if(code == upKey1) {
        up1 = state;
    } else if(code == downKey1) {
        down1 = state;
    } else if(code == leftKey1) {
        left1 = state;
    } else if(code == rightKey1) {
        right1 = state;
    } else if(code == attackKey1) {
        attack1 = state;
    } else if(code == dashKey1) {
        dash1 = state;
    } else if(code == upKey2) {
        up2 = state;
    } else if(code == downKey2) {
        down2 = state;
    } else if(code == leftKey2) {
        left2 = state;
    } else if(code == rightKey2) {
        right2 = state;
    } else if(code == attackKey2) {
        attack2 = state;
    } else if(code == dashKey2) {
        dash2 = state;
    }
}

void InputManager::playStateUpdate()
{
    // Move Player 1
    if(p1) {
        if(up1 && !down1) {
            if(left1 ^ right1) {
                if(left1)
                    player1->move(Character::Direction::UP_LEFT);
                else
                    player1->move(Character::Direction::UP_RIGHT);
            } else {
                player1->move(Character::Direction::UP);
            }
        } else if(down1 && !up1) {
            if(left1 ^ right1) {
                if(left1)
                    player1->move(Character::Direction::DOWN_LEFT);
                else
                    player1->move(Character::Direction::DOWN_RIGHT);
            } else {
                player1->move(Character::Direction::DOWN);
            }
        } else if(left1 ^ right1) {
            if(left1)
                player1->move(Character::Direction::LEFT);
            else
                player1->move(Character::Direction::RIGHT);
        } else {
            player1->stop();
        }

        if(dash1)
            player1->dash();
        else if(attack1)
            player1->attack();
    }

    // Move Player 2
    if(p2) {
        if(up2 && !down2) {
            if(left2 ^ right2) {
                if(left2)
                    player2->move(Character::Direction::UP_LEFT);
                else
                    player2->move(Character::Direction::UP_RIGHT);
            } else {
                player2->move(Character::Direction::UP);
            }
        } else if(down2 && !up2) {
            if(left2 ^ right2) {
                if(left2)
                    player2->move(Character::Direction::DOWN_LEFT);
                else
                    player2->move(Character::Direction::DOWN_RIGHT);
            } else {
                player2->move(Character::Direction::DOWN);
            }
        } else if(left2 ^ right2) {
            if(left2)
                player2->move(Character::Direction::LEFT);
            else
                player2->move(Character::Direction::RIGHT);
        } else {
            player2->stop();
        }

        if(dash2)
            player2->dash();
        else if(attack2)
            player2->attack();
    }
}

char *InputManager::SFKeyToString(unsigned int code) {
    char *keyStr = new char[20];
    if(code & 256) {
        code -= 256;

        int joystick = code & 128;
        if(code & 128) code -= 128;

        if(code & 64) {
            code -= 64;
            char sign = (code & 1 ? '-' : '+');
            code >>= 1;
            switch(code) {
                case sf::Joystick::X: sprintf(keyStr, "Joystick %i Axis %cX", joystick, sign); break;
                case sf::Joystick::Y: sprintf(keyStr, "Joystick %i Axis %cY", joystick, sign); break;
                case sf::Joystick::Z: sprintf(keyStr, "Joystick %i Axis %cZ", joystick, sign); break;
                case sf::Joystick::R: sprintf(keyStr, "Joystick %i Axis %cR", joystick, sign); break;
                case sf::Joystick::U: sprintf(keyStr, "Joystick %i Axis %cU", joystick, sign); break;
                case sf::Joystick::V: sprintf(keyStr, "Joystick %i Axis %cV", joystick, sign); break;
                case sf::Joystick::PovX: sprintf(keyStr, "Joystick %i Axis %cPovX", joystick, sign); break;
                case sf::Joystick::PovY: sprintf(keyStr, "Joystick %i Axis %cPovY", joystick, sign); break;
                default: sprintf(keyStr, "Invalid Key"); break;
            }
        } else {
            sprintf(keyStr, "Joystick %i Button %i", joystick, code);
        }
    } else if(code & 128) {
        code -= 128;
        switch(code) {
            case sf::Mouse::Left: sprintf(keyStr, "Left Mouse Button"); break;
            case sf::Mouse::Right: sprintf(keyStr, "Right Mouse Button"); break;
            case sf::Mouse::Middle: sprintf(keyStr, "Middle Mouse Button"); break;
            case sf::Mouse::XButton1: sprintf(keyStr, "Mouse Button 4"); break;
            case sf::Mouse::XButton2: sprintf(keyStr, "Mouse Button 5"); break;
            default: sprintf(keyStr, "Invalid Key"); break;
        }
    } else {
        if(code < 26) {
            sprintf(keyStr, "%c", code + 65);
            return keyStr;
        }
        switch (code) {
            case sf::Keyboard::Escape: sprintf(keyStr, "Escape"); break;
            case sf::Keyboard::LControl: sprintf(keyStr, "LControl"); break;
            case sf::Keyboard::LShift: sprintf(keyStr, "LShift"); break;
            case sf::Keyboard::LAlt: sprintf(keyStr, "LAlt"); break;
            case sf::Keyboard::LSystem: sprintf(keyStr, "LSystem"); break;
            case sf::Keyboard::RControl: sprintf(keyStr, "RControl"); break;
            case sf::Keyboard::RShift: sprintf(keyStr, "RShift"); break;
            case sf::Keyboard::RAlt: sprintf(keyStr, "RAlt"); break;
            case sf::Keyboard::RSystem: sprintf(keyStr, "RSystem"); break;
            case sf::Keyboard::Menu: sprintf(keyStr, "Menu"); break;
            case sf::Keyboard::LBracket: sprintf(keyStr, "LBracket"); break;
            case sf::Keyboard::RBracket: sprintf(keyStr, "RBracket"); break;
            case sf::Keyboard::SemiColon: sprintf(keyStr, ";"); break;
            case sf::Keyboard::Comma: sprintf(keyStr, ","); break;
            case sf::Keyboard::Period: sprintf(keyStr, "."); break;
            case sf::Keyboard::Quote: sprintf(keyStr, "\'"); break;
            case sf::Keyboard::Slash: sprintf(keyStr, "/"); break;
            case sf::Keyboard::BackSlash: sprintf(keyStr, "\\"); break;
            case sf::Keyboard::Tilde: sprintf(keyStr, "~"); break;
            case sf::Keyboard::Equal: sprintf(keyStr, "="); break;
            case sf::Keyboard::Dash: sprintf(keyStr, "-"); break;
            case sf::Keyboard::Space: sprintf(keyStr, "Space"); break;
            case sf::Keyboard::Return: sprintf(keyStr, "Return"); break;
            case sf::Keyboard::BackSpace: sprintf(keyStr, "Back"); break;
            case sf::Keyboard::Tab: sprintf(keyStr, "Tab"); break;
            case sf::Keyboard::PageUp: sprintf(keyStr, "Page Up"); break;
            case sf::Keyboard::PageDown: sprintf(keyStr, "Page Down"); break;
            case sf::Keyboard::End: sprintf(keyStr, "End"); break;
            case sf::Keyboard::Home: sprintf(keyStr, "Home"); break;
            case sf::Keyboard::Insert: sprintf(keyStr, "Insert"); break;
            case sf::Keyboard::Delete: sprintf(keyStr, "Delete"); break;
            case sf::Keyboard::Add: sprintf(keyStr, "+"); break;
            case sf::Keyboard::Subtract: sprintf(keyStr, "-"); break;
            case sf::Keyboard::Multiply: sprintf(keyStr, "*"); break;
            case sf::Keyboard::Divide: sprintf(keyStr, "/"); break;
            case sf::Keyboard::Left: sprintf(keyStr, "Left"); break;
            case sf::Keyboard::Right: sprintf(keyStr, "Right"); break;
            case sf::Keyboard::Up: sprintf(keyStr, "Up"); break;
            case sf::Keyboard::Down: sprintf(keyStr, "Down"); break;
            case sf::Keyboard::Numpad0: sprintf(keyStr, "NP 0"); break;
            case sf::Keyboard::Numpad1: sprintf(keyStr, "NP 1"); break;
            case sf::Keyboard::Numpad2: sprintf(keyStr, "NP 2"); break;
            case sf::Keyboard::Numpad3: sprintf(keyStr, "NP 3"); break;
            case sf::Keyboard::Numpad4: sprintf(keyStr, "NP 4"); break;
            case sf::Keyboard::Numpad5: sprintf(keyStr, "NP 5"); break;
            case sf::Keyboard::Numpad6: sprintf(keyStr, "NP 6"); break;
            case sf::Keyboard::Numpad7: sprintf(keyStr, "NP 7"); break;
            case sf::Keyboard::Numpad8: sprintf(keyStr, "NP 8"); break;
            case sf::Keyboard::Numpad9: sprintf(keyStr, "NP 9"); break;
            case sf::Keyboard::F1: sprintf(keyStr, "F1"); break;
            case sf::Keyboard::F2: sprintf(keyStr, "F2"); break;
            case sf::Keyboard::F3: sprintf(keyStr, "F3"); break;
            case sf::Keyboard::F4: sprintf(keyStr, "F4"); break;
            case sf::Keyboard::F5: sprintf(keyStr, "F5"); break;
            case sf::Keyboard::F6: sprintf(keyStr, "F6"); break;
            case sf::Keyboard::F7: sprintf(keyStr, "F7"); break;
            case sf::Keyboard::F8: sprintf(keyStr, "F8"); break;
            case sf::Keyboard::F9: sprintf(keyStr, "F9"); break;
            case sf::Keyboard::F10: sprintf(keyStr, "F10"); break;
            case sf::Keyboard::F11: sprintf(keyStr, "F11"); break;
            case sf::Keyboard::F12: sprintf(keyStr, "F12"); break;
            case sf::Keyboard::F13: sprintf(keyStr, "F13"); break;
            case sf::Keyboard::F14: sprintf(keyStr, "F14"); break;
            case sf::Keyboard::F15: sprintf(keyStr, "F15"); break;
            case sf::Keyboard::Pause: sprintf(keyStr, "Pause"); break;
            default: sprintf(keyStr, "Invalid Key"); break;
        }
    }

    return keyStr;
}

char *InputManager::up1String() { return SFKeyToString(upKey1); }
char *InputManager::down1String() { return SFKeyToString(downKey1); }
char *InputManager::left1String() { return SFKeyToString(leftKey1); }
char *InputManager::right1String() { return SFKeyToString(rightKey1); }
char *InputManager::attack1String() { return SFKeyToString(attackKey1); }
char *InputManager::dash1String() { return SFKeyToString(dashKey1); }

char *InputManager::up2String() { return SFKeyToString(upKey2); }
char *InputManager::down2String() { return SFKeyToString(downKey2); }
char *InputManager::left2String() { return SFKeyToString(leftKey2); }
char *InputManager::right2String() { return SFKeyToString(rightKey2); }
char *InputManager::attack2String() { return SFKeyToString(attackKey2); }
char *InputManager::dash2String() { return SFKeyToString(dashKey2); }

void InputManager::loadKeys()
{
    FILE *input = fopen("rec/keys.dat", "r");
    if(!input) {
        fprintf(stderr, "could not open file 'keys.dat'\n");
        return;
    }

	fscanf(input, "%i", &upKey1);
	fscanf(input, "%i", &downKey1);
	fscanf(input, "%i", &leftKey1);
	fscanf(input, "%i", &rightKey1);
	fscanf(input, "%i", &attackKey1);
	fscanf(input, "%i", &dashKey1);

	fscanf(input, "%i", &upKey2);
	fscanf(input, "%i", &downKey2);
	fscanf(input, "%i", &leftKey2);
	fscanf(input, "%i", &rightKey2);
	fscanf(input, "%i", &attackKey2);
	fscanf(input, "%i", &dashKey2);

    fclose(input);
}

void InputManager::saveKeys()
{
    FILE *output = fopen("rec/keys.dat", "w");
    if(!output) {
        fprintf(stderr, "could not open or create file 'keys.dat'\n");
        return;
    }

	fprintf(output, "%i\n", upKey1);
	fprintf(output, "%i\n", downKey1);
	fprintf(output, "%i\n", leftKey1);
	fprintf(output, "%i\n", rightKey1);
	fprintf(output, "%i\n", attackKey1);
	fprintf(output, "%i\n", dashKey1);

	fprintf(output, "%i\n", upKey2);
	fprintf(output, "%i\n", downKey2);
	fprintf(output, "%i\n", leftKey2);
	fprintf(output, "%i\n", rightKey2);
	fprintf(output, "%i\n", attackKey2);
	fprintf(output, "%i\n", dashKey2);

    fclose(output);
}
