#include "HUD.h"

HUD::HUD(Character *p1, Character *p2) : player1(p1), player2(p2)
{
}

HUD::~HUD() { };

void HUD::drawHUD(sf::RenderWindow *window, sf::Transform trans)
{
    // Draw player1 side
    {
        float h = player1->health * 2;
        sf::RectangleShape health(sf::Vector2f(20.f, h));
        health.setOrigin(0.f, h);

        float s = player1->stamina * 2;
        sf::RectangleShape stamina(sf::Vector2f(20.f, s));

        health.setFillColor(sf::Color(255, 64, 128));
        stamina.setFillColor(sf::Color(255, 128, 64));

        health.setPosition(102.f, 336.f);
        stamina.setPosition(102.f, 384.f);

        window->draw(health, trans);
        window->draw(stamina, trans);
    }

    // Draw player2 side
    {
        float h = player2->health * 2;
        sf::RectangleShape health(sf::Vector2f(20.f, h));
        health.setOrigin(0.f, h);

        float s = player2->stamina * 2;
        sf::RectangleShape stamina(sf::Vector2f(20.f, s));

        health.setFillColor(sf::Color(255, 64, 128));
        stamina.setFillColor(sf::Color(255, 128, 64));

        health.setPosition(1156.f, 336.f);
        stamina.setPosition(1156.f, 384.f);

        window->draw(health, trans);
        window->draw(stamina, trans);
    }
}
