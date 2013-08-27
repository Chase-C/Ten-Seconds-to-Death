#ifndef FIGHTER_H
#define FIGHTER_H

#include "Character.h"

class Fighter : public Character
{
    public:
        Fighter();
        ~Fighter();
        void Init(cpSpace *space, float x, float y, Direction d, sf::Color c);

        void dash();
        void attack();
        void move(Direction d);

        void ult();

        void update();
        void draw(sf::RenderWindow *window, sf::Transform trans);

    private:
        void checkDamage(float r2, int d, int s, float speed, float knockBack);
        void ultDamage(int a);

        sf::Sprite attackSprite[2];
        sf::Clock attackClock;
};

#endif
