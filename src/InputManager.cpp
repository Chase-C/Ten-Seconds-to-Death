#include <SFML/Graphics.hpp>
#include <cmath>

#include "InputManager.h"
#include "Character.h"

InputManager::InputManager(Character *c1, Character *c2)
{
    player1 = c1;
    player2 = c2;

	up = false;
	down = false;
	left = false;
	right = false;
    w = false;
    a = false;
    s = false;
    d = false;
    shift = false;
    control = false;
	esc = false;

    rClick = false;
    lClick = false;
}

void InputManager::CheckKeyPress(int keyCode)
{
	if(keyCode == sf::Keyboard::Up) {
		up = true;
	} else if(keyCode == sf::Keyboard::Down) {
		down = true;
	} else if(keyCode == sf::Keyboard::Left) {
		left = true;
	} else if(keyCode == sf::Keyboard::Right) {
		right = true;
	} else if(keyCode == sf::Keyboard::W) {
		w = true;
	} else if(keyCode == sf::Keyboard::A) {
		a = true;
	} else if(keyCode == sf::Keyboard::S) {
		s = true;
    } else if(keyCode == sf::Keyboard::D) {
		d = true;
    } else if(keyCode == sf::Keyboard::LShift) {
		shift = true;
    } else if(keyCode == sf::Keyboard::LControl) {
		control = true;
    } else if(keyCode == sf::Keyboard::Escape) {
		esc = true;
	}
}

void InputManager::CheckKeyRelease(int keyCode)
{
	if(keyCode == sf::Keyboard::Up) {
		up = false;
	} else if(keyCode == sf::Keyboard::Down) {
		down = false;
	} else if(keyCode == sf::Keyboard::Left) {
		left = false;
	} else if(keyCode == sf::Keyboard::Right) {
		right = false;
	} else if(keyCode == sf::Keyboard::W) {
		w = false;
	} else if(keyCode == sf::Keyboard::A) {
		a = false;
	} else if(keyCode == sf::Keyboard::S) {
		s = false;
    } else if(keyCode == sf::Keyboard::D) {
		d = false;
    } else if(keyCode == sf::Keyboard::LShift) {
		shift = false;
    } else if(keyCode == sf::Keyboard::LControl) {
		control = false;
    } else if(keyCode == sf::Keyboard::Escape) {
		esc = false;
    }
}

void InputManager::mouseUpdate()
{
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        lClick = true;
    else
        lClick = false;
    
    if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        rClick = true;
    else
        rClick = false;
}

void InputManager::Update()
{
    // Move Player 1
    {
        if(w && !s) {
            if(a ^ d) {
                if(a)
                    player1->move(Character::Direction::UP_LEFT);
                else
                    player1->move(Character::Direction::UP_RIGHT);
            } else {
                player1->move(Character::Direction::UP);
            }
        } else if(s && !w) {
            if(a ^ d) {
                if(a)
                    player1->move(Character::Direction::DOWN_LEFT);
                else
                    player1->move(Character::Direction::DOWN_RIGHT);
            } else {
                player1->move(Character::Direction::DOWN);
            }
        } else if(a ^ d) {
            if(a)
                player1->move(Character::Direction::LEFT);
            else
                player1->move(Character::Direction::RIGHT);
        } else {
            player1->stop();
        }

        if(shift)
            player1->dash();
        else if(control)
            player1->attack();
    }

    // Move Player 2
    {
        if(up && !down) {
            if(left ^ right) {
                if(left)
                    player2->move(Character::Direction::UP_LEFT);
                else
                    player2->move(Character::Direction::UP_RIGHT);
            } else {
                player2->move(Character::Direction::UP);
            }
        } else if(down && !up) {
            if(left ^ right) {
                if(left)
                    player2->move(Character::Direction::DOWN_LEFT);
                else
                    player2->move(Character::Direction::DOWN_RIGHT);
            } else {
                player2->move(Character::Direction::DOWN);
            }
        } else if(left ^ right) {
            if(left)
                player2->move(Character::Direction::LEFT);
            else
                player2->move(Character::Direction::RIGHT);
        } else {
            player2->stop();
        }

        if(rClick)
            player2->dash();
        else if(lClick)
            player2->attack();
    }
}
