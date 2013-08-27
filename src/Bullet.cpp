#include "Bullet.h"

Bullet::Bullet(float _x, float _y, float _r2, float xv, float yv, float rot, sf::Texture *laserball, bool u) :
    x(_x), y(_y), r2(_r2), xVel(xv), yVel(yv), ult(u)
{
    anim.setSpriteSheet(*laserball);
    for(int i = 0; i < 4; i++)
        anim.addFrame(sf::IntRect(32 * i, 0, 32, 32));

    sprite.setFrameTime(sf::seconds(0.10));
    sprite.setAnimation(anim);
    sprite.setOrigin(16, 16);
    sprite.setLooped(false);
    sprite.setRotation(rot);
    if(r2 > 4000)
        sprite.setScale(sf::Vector2f(4.f, 4.f));
    sprite.stop();

    exploding = false;
    destroyed = false;
}

void Bullet::update(sf::Time t)
{
    x += xVel;
    y += yVel;

    sprite.update(t);

    if(!sprite.isPlaying() && exploding)
        destroyed = true;
}

cpVect Bullet::getPosition()
{
    return cpv(x, y);
}

float Bullet::getR2()
{
    return r2;
}

cpVect Bullet::getVelocity()
{
    return cpv(xVel, yVel);
}

Bullet::~Bullet()
{
}

void Bullet::explode()
{
    sprite.play();
    exploding = true;
    xVel /= 2;
    yVel /= 2;
}

void Bullet::draw(sf::RenderWindow *window, sf::Transform trans)
{
    sprite.setPosition(x, y);
    window->draw(sprite, trans);
}
