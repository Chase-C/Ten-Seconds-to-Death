#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>
#include "Character.h"

class Camera
{
public:
    Camera() { };
	Camera(Character *c);
	~Camera();

    void setFocus(Character *c);
    void update();

    sf::View getView();

private:
    Character *focus;
    sf::View view;
};

#endif
