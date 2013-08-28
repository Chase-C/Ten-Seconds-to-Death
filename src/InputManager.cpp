#include <SFML/Graphics.hpp>
#include <cmath>

#include "InputManager.h"
#include "Character.h"

InputManager::InputManager(Character *c1, Character *c2)
{
    player1 = c1;
    player2 = c2;
    p2 = true;

    init();
}

InputManager::InputManager(Character *c1)
{
    player1 = c1;
    player2 = NULL;
    p2 = false;

    init();
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

void InputManager::eventUpdate(int keyCode, bool state)
{
    if(keyCode == upKey1) {
        up1 = state;
    } else if(keyCode == downKey1) {
        down1 = state;
    } else if(keyCode == leftKey1) {
        left1 = state;
    } else if(keyCode == rightKey1) {
        right1 = state;
    } else if(keyCode == attackKey1) {
        attack1 = state;
    } else if(keyCode == dashKey1) {
        dash1 = state;
    } else if(keyCode == upKey2) {
        up2 = state;
    } else if(keyCode == downKey2) {
        down2 = state;
    } else if(keyCode == leftKey2) {
        left2 = state;
    } else if(keyCode == rightKey2) {
        right2 = state;
    } else if(keyCode == attackKey2) {
        attack2 = state;
    } else if(keyCode == dashKey2) {
        dash2 = state;
    }
}

void InputManager::Update()
{
    // Move Player 1
    {
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
