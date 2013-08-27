#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <chipmunk/chipmunk.h>
#include "AnimatedSprite.hpp"

class Character
{
public:
    enum Direction {
        LEFT = 0, UP_LEFT, UP, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT
    };

    Character();
	virtual ~Character();
    virtual void Init(cpSpace *space, float x, float y, Direction d, sf::Color c) { };
    void ReInit(float x, float y, Direction d);

    enum Type {
        FIGHTER, SHOOTER, NINJA
    } type;

    bool isDead();

    void setOponent(Character *o);

    virtual void update() { };
    void physUpdate();

    virtual void move(Direction d) { };
    void stop();
    virtual void dash() { };
    virtual void attack() { };
    void damage(float hAmount, int sAmount, float speed, cpVect knockBack);

    virtual void ult() { };

    cpVect getPosition();

    virtual void draw(sf::RenderWindow *window, sf::Transform trans) { };

protected:
    bool dead;
    Character *op;

    Direction dir;

    void drawArrow(sf::RenderWindow *window, sf::Transform trans);
    sf::Color arrowColor;

    enum CharacterState {
        STANDING, WALKING, ATTACKING, DASHING, DAMAGED, ULT
    } state;

    cpVect baseVelocity();

    float maxRunVelocity;
    float attackVelocity;
    int attacks;
    int attackFrames;
    float maxDashVelocity;
    int dashFrames;
    float speedMod;

    float health;
    int stamina;

    sf::Time staminaTime;
    sf::Clock staminaFillTimer;

    size_t walkFrame;
    sf::Texture *texture;

    Animation walking;
    AnimatedSprite walkSprite;

    Animation dashing;
    AnimatedSprite dashSprite;

    Animation ulting;
    AnimatedSprite ultSprite;
    sf::Clock ultClock;
    int ultLevel;
    bool ultToggle;

    sf::Clock frameClock;

    sf::SoundBuffer buffer;
    sf::Sound shoot;

    cpBody *body;
    cpConstraint *pivot;
    cpConstraint *gear;

    std::vector<cpShape*> shapes;

    friend class HUD;
};

#endif
