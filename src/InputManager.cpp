#include <SFML/Graphics.hpp>
#include <cmath>

#include "InputManager.h"
#include "Character.h"

InputManager::InputManager(Character* c)
{
    character = c;

	up = false;
	down = false;
	left = false;
	right = false;
	pause = false;

	upKeys[0] = sf::Keyboard::W;
	upKeys[1] = sf::Keyboard::Up;
	downKeys[0] = sf::Keyboard::S;
	downKeys[1] = sf::Keyboard::Down;
	leftKeys[0] = sf::Keyboard::A;
	leftKeys[1] = sf::Keyboard::Left;
	rightKeys[0] = sf::Keyboard::D;
	rightKeys[1] = sf::Keyboard::Right;
	pauseKeys[0] = sf::Keyboard::Escape;
	pauseKeys[1] = -1;

	lClick = false;
	rClick = false;
}

void InputManager::CheckKeyPress(int keyCode)
{
	if(keyCode == upKeys[0] || keyCode == upKeys[1]) {				//	^
		up = true;
	} else if(keyCode == downKeys[0] || keyCode == downKeys[1]) {	//	\/
		down = true;
	} else if(keyCode == leftKeys[0] || keyCode == leftKeys[1]) {	//	<-
		left = true;
	} else if(keyCode == rightKeys[0] || keyCode == rightKeys[1]) {	//	->
		right = true;
    } else if(keyCode == pauseKeys[0] || keyCode == pauseKeys[1]) {	//	pause
		pause = true;
	}
}

void InputManager::CheckKeyRelease(int keyCode)
{
	if(keyCode == upKeys[0] || keyCode == upKeys[1]) {				//	^
		up = false;
	} else if(keyCode == downKeys[0] || keyCode == downKeys[1]) {	//	\/
		down = false;
	} else if(keyCode == leftKeys[0] || keyCode == leftKeys[1]) {	//	<-
		left = false;
	} else if(keyCode == rightKeys[0] || keyCode == rightKeys[1]) {	//	->
		right = false;
	} else if(keyCode == pauseKeys[0] || keyCode == pauseKeys[1]) {	//	pause
		pause = false;
	}
}

void InputManager::mousePress(Engine *game)
{
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        lClick = true;
    } else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
        rClick = true;
        character->move(game->window.mapPixelToCoords(sf::Mouse::getPosition(game->window)));
    }
}

void InputManager::Update()
{
}
