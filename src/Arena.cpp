#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <chipmunk/chipmunk.h>

#include "Arena.h"

Arena::Arena(cpSpace *s)
{
    space = s;

    texture = new sf::Texture;
    if(!texture->loadFromFile("rec/map.png")) {
        fprintf(stderr, "could not load 'map.png'\n");
        return;
    }

    sprite.setTexture(*texture);
    sprite.setPosition(sf::Vector2f(0.f, 0.f));
    sprite.setOrigin(sf::Vector2f(0.f, 0.f));

    cpShape *wall[4] = {
        cpSegmentShapeNew(space->staticBody, cpv(190, 35), cpv(1090, 35), 0),
        cpSegmentShapeNew(space->staticBody, cpv(1090, 35), cpv(1090, 685), 0),
        cpSegmentShapeNew(space->staticBody, cpv(190, 685), cpv(1090, 685), 0),
        cpSegmentShapeNew(space->staticBody, cpv(190, 35), cpv(190, 685), 0)
    };

    for(int i = 0; i < 4; i++) {
        cpShapeSetFriction(wall[i], 0.7f);
        cpSpaceAddShape(space, wall[i]);
    }
}

void Arena::drawArena(sf::RenderWindow *window, sf::Transform trans)
{
    /*sf::RectangleShape rect(sf::Vector2f(900.f, 650.f));
    rect.setFillColor(sf::Color(64, 255, 128));
    rect.setPosition(150, 35);*/
    window->draw(sprite, trans);
}

Arena::~Arena()
{
}
