#ifndef SHOOTER_H
#define SHOOTER_H

#include "Character.h"
#include "Bullet.h"

class Shooter : public Character
{
    public:
        Shooter();
        ~Shooter();
        void Init(cpSpace *space, float x, float y, Direction d, sf::Color c);
        void ReInit(float x, float y, Direction d);

        void dash();
        void attack();
        void move(Direction d);

        void ult();

        void update();
        void draw(sf::RenderWindow *window, sf::Transform trans);
        void drawBullets(sf::RenderWindow *window, sf::Transform trans);

    private:
        std::vector<Bullet*> bullets;

        cpVect firePos();
        void fireBullet();

        void fireUlt1();
        void fireUlt2();

        void checkBulletCollision(Bullet *b, Character *c);
        void checkUltCollisions();

        Animation attacking;
        AnimatedSprite attackSprite;

        sf::Texture *ultraTexture;
        sf::Sprite ultraSprite;

        sf::Texture *laserball;

        sf::SoundBuffer lbuffer;
        sf::Sound laser;
        bool playLaser;
};

#endif
