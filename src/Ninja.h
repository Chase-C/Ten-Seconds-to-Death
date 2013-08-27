#ifndef NINJA_H
#define NINJA_H

#include "Character.h"

class Ninja : public Character
{
    public:
        Ninja();
        ~Ninja();
        void Init(cpSpace *space, float x, float y, Direction d, sf::Color c);

        void dash();
        void attack();
        void move(Direction d);

        void ult();

        void update();
        void draw(sf::RenderWindow *window, sf::Transform trans);

    private:
        void checkDamage(cpVect from, float r2, int d, int s, float speed, float knockBack);
        void ultAttack1();
        void ultAttack2();

        Animation attacking;
        AnimatedSprite attackSprite; 
};

#endif
