#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include <chipmunk/chipmunk.h>
#include "AnimatedSprite.hpp"

class Character
{
public:
	Character(cpSpace *space, float x, float y);
	~Character();

    void update();
    void physUpdate();

    void move(sf::Vector2f pt);
    void attack();

    cpVect getPosition();

    void draw(sf::RenderWindow *window, sf::Transform trans);

private:

    void changeDirection();

    enum Direction {
        LEFT, UP, RIGHT, DOWN
        //UP_L, UP_R, DOWN_L, DOWN_R;
    } dir;

    enum CharacterState {
        STANDING, WALKING, ATTACKING
    } state;

    float maxRunVelocity;

    cpVect runDirection;
    cpVect runPoint;

    std::vector<cpVect> path;
    unsigned int pathSeg;

    //sf::Texture texture;
    //sf::RenderTexture renderTexture;
    sf::Texture texture;
    Animation walking[4];
    AnimatedSprite walkSprite[4];

    Animation standing[4];
    AnimatedSprite standSprite[4];

    Animation attacking[4];
    AnimatedSprite attackSprite[4];

    sf::Clock frameClock;

    cpBody *body;
    std::vector<cpShape*> shapes;
};

#endif
