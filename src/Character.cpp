#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <chipmunk/chipmunk.h>
#include <cmath>

#include "Character.h"
#include "GPlayState.h"
#include "Arena.h"

Character::Character()
{
}

Character::~Character()
{
    cpBodyFree(body);
    for(cpShape *s : shapes)
        cpShapeFree(s);
}

void Character::ReInit(float x, float y, Direction d)
{
    body->p = cpv(x, y);
    dir = d;
    state = STANDING;

    cpConstraintSetMaxForce(pivot, 1200.0f); // emulate linear friction

    dashFrames = 0;
    speedMod = 0.f;
    ultLevel = 0;
    ultToggle = false;

    dead = false;
    health = 100;
    stamina = 100;

    staminaTime = sf::milliseconds(0);
    staminaFillTimer.restart();
    frameClock.restart();
}

bool Character::isDead()
{
    return dead;
}

void Character::setOponent(Character *o)
{
    op = o;
}

void Character::stop()
{
    switch(state) {
        case WALKING:
            walkSprite.stop();
            walkSprite.setFrame(0);
            break;
        case DASHING:
        case ATTACKING:
        case STANDING:
        default:
            return;
    }
    state = STANDING;
}

void Character::damage(float hAmount, int sAmount, float speed, cpVect knockBack)
{
    health -= hAmount;
    stamina -= sAmount;

    if(health < 0)
        health = 0;

    if(health == 0 && speed != 0.f)
        dead = true;

    if(stamina < 0)
        stamina = 0;

    speedMod = speed;
    cpBodySetVel(body, body->v + knockBack);
}

void Character::physUpdate()
{
    switch(state) {
        case DASHING:
            body->v = body->v * 512;
            body->v = cpvclamp(body->v, (speedMod + 1) * maxDashVelocity);
            break;
        case ATTACKING:
            body->v = cpvclamp(body->v, (speedMod + 1) * (maxRunVelocity + attackVelocity));
            break;
        case WALKING:
            body->v = body->v * 256;
        case STANDING:
        default:
            body->v = cpvclamp(body->v, (speedMod + 1) * maxRunVelocity);
            break;
    }

    if(speedMod != 0.f) speedMod *= .95;
}

cpVect Character::baseVelocity()
{
    cpVect vel;
    switch(dir) {
        case LEFT:
            vel = cpv(-1.f, 0.f);
            break;
        case UP_LEFT:
            vel = cpv(-1.f, -1.f);
            break;
        case UP_RIGHT:
            vel = cpv(1.f, -1.f);
            break;
        case UP:
            vel = cpv(0.f, -1.f);
            break;
        case RIGHT:
            vel = cpv(1.f, 0.f);
            break;
        case DOWN_RIGHT:
            vel = cpv(1.f, 1.f);
            break;
        case DOWN:
            vel = cpv(0.f, 1.f);
            break;
        case DOWN_LEFT:
            vel = cpv(-1.f, 1.f);
            break;
    }

    return cpvnormalize(vel);
}

cpVect Character::getPosition()
{
    return body->p;
}

void Character::drawArrow(sf::RenderWindow *window, sf::Transform trans)
{
    sf::CircleShape triangle(5, 3);
    triangle.setFillColor(arrowColor);
    triangle.setOrigin(5, 5);
    triangle.setPosition(body->p.x, body->p.y);

    float rotation = 0.f;
    float x = 0.f, y = 0.f;
    switch(dir) {
        case LEFT:
            x = -30.f;
            rotation = -90.f;
            break;
        case UP_LEFT:
            x = -20.f;
            y = -20.f;
            rotation = -45.f;
            break;
        case UP:
            y = -30.f;
            break;
        case UP_RIGHT:
            x = 20.f;
            y = -20.f;
            rotation = 45.f;
            break;
        case RIGHT:
            x = 30.f;
            rotation = 90.f;
            break;
        case DOWN_RIGHT:
            x = 20.f;
            y = 20.f;
            rotation = 135.f;
            break;
        case DOWN:
            y = 30.f;
            rotation = 180.f;
            break;
        case DOWN_LEFT:
            x = -20.f;
            y = 20.f;
            rotation = -135.f;
            break;
    }

    triangle.setRotation(rotation);
    triangle.move(x, y);

    window->draw(triangle, trans);
}
