#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <chipmunk/chipmunk.h>
#include <cmath>

#include "Character.h"
#include "GPlayState.h"
#include "Terrain.h"

Character::Character(cpSpace *space, float x, float y)
{
    if(!texture.loadFromFile("zombie.png")) {
        fprintf(stderr, "could not load 'zombie.png'\n");
    }

    for(int i = 0; i < 4; i++) {
        walking[i].setSpriteSheet(texture);
        for(int j = 0; j < 8; j++)
            walking[i].addFrame(sf::IntRect((128 * j) + 512, 256 * i, 128, 128));

        walkSprite[i].setFrameTime(sf::seconds(0.12));
        walkSprite[i].setAnimation(walking[i]);
        walkSprite[i].setOrigin(64, 64);
    }

    for(int i = 0; i < 4; i++) {
        standing[i].setSpriteSheet(texture);
        for(int j = 0; j < 4; j++)
            standing[i].addFrame(sf::IntRect(128 * j, 256 * i, 128, 128));

        standSprite[i].setFrameTime(sf::seconds(0.2));
        standSprite[i].setAnimation(standing[i]);
        standSprite[i].setOrigin(64, 64);
    }

    for(int i = 0; i < 4; i++) {
        attacking[i].setSpriteSheet(texture);
        for(int j = 0; j < 10; j++)
            attacking[i].addFrame(sf::IntRect((128 * j) + 1024, 256 * i, 128, 128));

        attackSprite[i].setFrameTime(sf::seconds(0.12));
        attackSprite[i].setAnimation(attacking[i]);
        attackSprite[i].setOrigin(64, 64);
    }

    dir = DOWN;
    state = STANDING;
    standSprite[dir].play();

    runPoint = cpv(x, y);
    runDirection = cpvzero;

    double moment = cpMomentForCircle(1, 0, 16, cpvzero);
    body = cpSpaceAddBody(space, cpBodyNew(1, moment));
    cpBodySetPos(body, cpv(x, y));

	cpBody *staticBody = cpSpaceGetStaticBody(space);
    cpConstraint *pivot = cpSpaceAddConstraint(space, cpPivotJointNew2(staticBody, body, cpvzero, cpvzero));
    cpConstraintSetMaxBias(pivot, 0); // disable joint correction
    cpConstraintSetMaxForce(pivot, 1000.0f); // emulate linear friction

    cpConstraint *gear = cpSpaceAddConstraint(space, cpGearJointNew(staticBody, body, 0.0f, 1.0f));
    cpConstraintSetMaxBias(gear, 0); // disable joint correction
    cpConstraintSetMaxForce(gear, 5000.0f); // emulate angular friction

    maxRunVelocity = 128;

    shapes.push_back(cpSpaceAddShape(space, cpCircleShapeNew(body, 16, cpvzero)));
    cpShapeSetFriction(shapes.back(), 0.7f);

    frameClock.restart();
}

Character::~Character()
{
    cpBodyFree(body);
    for(cpShape *s : shapes)
        cpShapeFree(s);
}

void Character::move(sf::Vector2f pt)
{
    Terrain *terrain = GPlayState::Instance()->getTerrain();

    cpVect dest = terrain->tileAtCoords(pt);
    sf::Vector2f playerPos(body->p.x, body->p.y);
    cpVect start = terrain->tileAtCoords(playerPos);

    path = terrain->getPath(start, dest);
    if(path.size() == 0) return;

    for(int i = 0; i < path.size(); i++)
        printf("(%g, %g)\n", path[i].x, path[i].y);

    pathSeg = 1;
    changeDirection();
}

void Character::attack()
{
}

void Character::update()
{
    if(state == WALKING && cpvnear(body->p, runPoint, 16)) {
        if(path.size() > ++pathSeg) {
            changeDirection();
        } else {
            cpBodySetVel(body, cpvzero);
            runDirection = cpvzero;
            state = STANDING;
            walkSprite[dir].stop();
            standSprite[dir].play();
        }
    }

    switch(state) {
        case STANDING:
            standSprite[dir].update(frameClock.restart());
            break;
        case WALKING:
            walkSprite[dir].update(frameClock.restart());
            break;
        case ATTACKING:
            attackSprite[dir].update(frameClock.restart());
            break;
        default:
            break;
    }
}

void Character::physUpdate()
{
    if(!(runDirection == cpvzero)) {
        body->v = runDirection * 128;
        body->v = cpvclamp(body->v, maxRunVelocity);
    }
}

cpVect Character::getPosition()
{
    return body->p;
}

void Character::changeDirection()
{
    if(path.size() < pathSeg + 1) return;

    path[pathSeg - 1] = body->p;
    path[pathSeg] = path[pathSeg] * 32;
    runPoint = path[pathSeg];

    runDirection = cpv(path[pathSeg].x - path[pathSeg - 1].x, path[pathSeg].y - path[pathSeg - 1].y);
    cpvnormalize(runDirection);

    Direction oldDir = dir;

    if(abs(runDirection.x) > abs(runDirection.y)) {
        if(runDirection.x > 0)
            dir = RIGHT;
        else
            dir = LEFT;
    } else {
        if(runDirection.y > 0)
            dir = DOWN;
        else
            dir = UP;
    }

    switch(state) {
        case STANDING:
            standSprite[oldDir].stop();
            break;
        case WALKING:
            if(oldDir != dir)
                walkSprite[oldDir].stop();
            break;
        case ATTACKING:
            attackSprite[oldDir].stop();
            break;
        default:
            break;
    }

    walkSprite[dir].play();
    state = WALKING;
}

void Character::draw(sf::RenderWindow *window, sf::Transform trans)
{
    cpVect pos = cpBodyGetPos(body);
    switch(state) {
        case STANDING:
            standSprite[dir].setPosition(pos.x, pos.y);
            window->draw(standSprite[dir], trans);
            break;
        case WALKING:
            walkSprite[dir].setPosition(pos.x, pos.y);
            window->draw(walkSprite[dir], trans);
            break;
        case ATTACKING:
            attackSprite[dir].setPosition(pos.x, pos.y);
            window->draw(attackSprite[dir], trans);
            break;
        default:
            break;
    }
}
