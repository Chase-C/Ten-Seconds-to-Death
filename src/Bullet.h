#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <chipmunk/chipmunk.h>
#include "AnimatedSprite.hpp"

class Bullet
{
    public:
        Bullet() { };
        Bullet(float _x, float _y, float _r2, float vx, float vy, float rot, sf::Texture *laserball, bool u);
        ~Bullet();

        void update(sf::Time t);
        cpVect getPosition();
        cpVect getVelocity();
        float getR2();

        bool destroyed;
        bool exploding;

        void explode();
        void draw(sf::RenderWindow *window, sf::Transform trans);

        bool ult;

    private:
        float x, y;
        float xVel, yVel;
        float r2;

        Animation anim;
        AnimatedSprite sprite;
};

#endif
