#include <SFML/Graphics.hpp>

#include "Camera.h"

Camera::Camera(Character *c)
{
    focus = c;
    cpVect pos = focus->getPosition();
    view.setCenter(sf::Vector2f(pos.x, pos.y));
    view.setSize(sf::Vector2f(800, 600));
}

Camera::~Camera()
{
}

void Camera::setFocus(Character *c)
{
    focus = c;
    cpVect pos = focus->getPosition();
    view.setCenter(sf::Vector2f(pos.x, pos.y));
    view.setSize(sf::Vector2f(800, 600));
}

void Camera::update()
{
    cpVect pos = focus->getPosition();
    view.setCenter(sf::Vector2f(pos.x, pos.y));
}

sf::View Camera::getView()
{
    return view;
}
