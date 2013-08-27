#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>
#include "Character.h"

class HUD
{
    public:
        HUD() { };
        HUD(Character *p1, Character *p2);
        ~HUD();

        void drawHUD(sf::RenderWindow *window, sf::Transform trans);

    private:
        Character *player1;
        Character *player2;
};

#endif
