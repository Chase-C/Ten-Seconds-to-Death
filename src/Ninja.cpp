#include "Ninja.h"

Ninja::Ninja()
{
    type = NINJA;
}

void Ninja::Init(cpSpace *space, float x, float y, Direction d, sf::Color c)
{
    texture = new sf::Texture();
    if(!texture->loadFromFile("rec/elektro.png")) {
        fprintf(stderr, "could not load 'walking.png'\n");
        return;
    }

    if(!buffer.loadFromFile("rec/whip.wav")) {
        fprintf(stderr, "could not load file 'shoot.wav'\n");
    }
    shoot.setBuffer(buffer);
    shoot.setVolume(60);

    arrowColor = c;
    dir = d;

    texture->setSmooth(true);

    walking.setSpriteSheet(*texture);
    for(int i = 0; i < 8; i++)
        walking.addFrame(sf::IntRect(128 * i, 0, 128, 128));

    walkSprite.setFrameTime(sf::seconds(0.10));
    walkSprite.setAnimation(walking);
    walkSprite.setOrigin(64, 64);

    dashing.setSpriteSheet(*texture);
    for(int i = 0; i < 3; i++)
        dashing.addFrame(sf::IntRect((128 * i) + 1024, 0, 128, 128));

    dashSprite.setFrameTime(sf::seconds(0.10));
    dashSprite.setAnimation(dashing);
    dashSprite.setOrigin(64, 64);

    attacking.setSpriteSheet(*texture);
    for(int i = 0; i < 3; i++)
        attacking.addFrame(sf::IntRect((128 * i) + 1408, 0, 128, 128));

    attackSprite.setFrameTime(sf::seconds(0.10));
    attackSprite.setAnimation(attacking);
    attackSprite.setOrigin(64, 64);
    attackSprite.setLooped(false);
    attackSprite.stop();

    ulting.setSpriteSheet(*texture);
    for(int i = 0; i < 12; i++)
        ulting.addFrame(sf::IntRect((128 * i), 128, 128, 128));

    ultSprite.setFrameTime(sf::seconds(0.12));
    ultSprite.setAnimation(ulting);
    ultSprite.setOrigin(64, 64);
    ultSprite.setLooped(false);

    state = STANDING;

    double moment = cpMomentForCircle(1, 0, 16, cpvzero);
    body = cpSpaceAddBody(space, cpBodyNew(1, moment));
    cpBodySetPos(body, cpv(x, y));

    cpBody *staticBody = cpSpaceGetStaticBody(space);
    pivot = cpSpaceAddConstraint(space, cpPivotJointNew2(staticBody, body, cpvzero, cpvzero));
    cpConstraintSetMaxBias(pivot, 0); // disable joint correction
    cpConstraintSetMaxForce(pivot, 1200.0f); // emulate linear friction

    gear = cpSpaceAddConstraint(space, cpGearJointNew(staticBody, body, 0.0f, 1.0f));
    cpConstraintSetMaxBias(gear, 0); // disable joint correction
    cpConstraintSetMaxForce(gear, 5000.0f); // emulate angular friction

    maxRunVelocity = 256;
    attackVelocity = 128;
    maxDashVelocity = 512;
    dashFrames = 0;
    speedMod = 0.f;
    ultLevel = 0;
    ultToggle = false;

    dead = false;
    health = 100;
    stamina = 100;

    shapes.push_back(cpSpaceAddShape(space, cpCircleShapeNew(body, 16, cpvzero)));
    cpShapeSetFriction(shapes.back(), 0.7f);

    staminaTime = sf::milliseconds(0);
    staminaFillTimer.restart();
    frameClock.restart();
}

Ninja::~Ninja()
{
}

void Ninja::dash()
{
    if(dashFrames <= 0 && stamina >= 50) {
        switch(state) {
            case WALKING:
                walkSprite.stop();
                break;
            case STANDING:
            case ATTACKING:
                attackSprite.stop();
                break;
            case DASHING:
            case ULT:
            default:
                return;
        }
        cpVect vel = baseVelocity() * maxDashVelocity;
        vel = cpvclamp(vel, maxDashVelocity);
        cpBodySetVel(body, vel);

        state = DASHING;
        dashSprite.play();
        dashFrames = 25;

        stamina -= 50;
    }
}

void Ninja::attack()
{
    if(state == ULT) {
        if(ultLevel == 0 && ultClock.getElapsedTime() > sf::milliseconds(0))
            ultLevel++;
        else if(ultLevel == 1 && ultClock.getElapsedTime() > sf::milliseconds(360))
            ultLevel++;
    } else if(!attackSprite.isPlaying()) {
        switch(state) {
            case WALKING:
                walkSprite.stop();
                walkSprite.setFrame(0);
            case STANDING:
                break;
            case ATTACKING:
            case DASHING:
            case ULT:
            default:
                return;
        }

        cpVect vel = baseVelocity() * attackVelocity;
        vel = cpvclamp(vel, attackVelocity);

        cpBodySetVel(body, body->v + vel);

        state = ATTACKING;

        attackSprite.play();
        cpVect damageArea = body->p + (baseVelocity() * 64);
        checkDamage(damageArea, 3600.f, 5, 50, 0.f, 0.f);
    }
}

void Ninja::move(Direction d)
{
    switch(state) {
        case STANDING:
            //standSprite.stop();
            break;
        case WALKING:
            if(dir == d) return;
            break;
        case ATTACKING:
        case DASHING:
        case ULT:
        default:
            return;
    }

    state = WALKING;

    dir = d;
    walkSprite.play();

    cpVect vel = baseVelocity() * maxRunVelocity;
    vel = cpvclamp(vel, maxRunVelocity);
    cpBodySetVel(body, vel);
}

void Ninja::ult()
{
    switch(state) {
        case WALKING:
            walkSprite.stop();
            break;
        case DASHING:
            dashFrames = 0;
            dashSprite.stop();
            break;
        case ATTACKING:
            attackSprite.stop();
        case STANDING:
        default:
            break;
    }          

    state = ULT;

    cpVect vel = baseVelocity() * attackVelocity;
    vel = cpvclamp(vel, maxRunVelocity);
    cpBodySetVel(body, vel);

    ultSprite.play();
    ultClock.restart();
}

void Ninja::update()
{
    if(dashFrames > 0)
        dashFrames--;

    switch(state) {
        case WALKING:
            walkSprite.update(frameClock.restart());
            break;
        case ATTACKING:
            attackSprite.update(frameClock.restart());
            if(!attackSprite.isPlaying()) {
                state = STANDING;
                attackSprite.stop();
            }
            break;
        case DASHING:
            dashSprite.update(frameClock.restart());
            if(dashFrames <= 10) {
                state = STANDING;
                dashSprite.stop();
            }
            break;
        case ULT:
            {
                ultSprite.update(frameClock.restart());
                sf::Time t = ultClock.getElapsedTime();
                int frame = ultSprite.getFrame();
                if(t > sf::seconds(1.56) || (frame >= 6 && ultLevel < 2)
                        || (frame >= 3 && ultLevel < 1)) {
                    ultSprite.stop();
                    ultLevel = 0;
                    state = STANDING;
                    ultToggle = false;
                }

                if(frame == 3 && !ultToggle) {
                    ultAttack1();
                    ultToggle = true;
                } else if(frame == 6 && ultToggle) {
                    if(stamina > 30) {
                        ultAttack1();
                        stamina -= 30;
                    } else ultLevel--;
                    ultToggle = false;
                } else if(frame > 6) {
                    if(stamina > 0) {
                        ultAttack2();
                        stamina -= 1;
                    } else ultLevel--;
                }
            }
            break;
        case STANDING:
        default:
            break;
    } 

    staminaTime += staminaFillTimer.restart();
    if(staminaTime.asMilliseconds() > 40) {
        if(stamina < 100) stamina++;
        staminaTime -= sf::milliseconds(40);
    }
}

void Ninja::ultAttack1()
{
    cpVect damageArea = body->p + (baseVelocity() * 64.f);
    checkDamage(damageArea, 4096.f, 20, 40, -.6f, 0.f);
}

void Ninja::ultAttack2()
{
    checkDamage(body->p, 17956.f, 2, 4, -.6f, 256.f);
}

void Ninja::checkDamage(cpVect from, float r2, int d, int s, float speed, float knockBack)
{
    float xDist = from.x - op->getPosition().x;
    float yDist = from.y - op->getPosition().y;

    shoot.play();

    cpVect k = cpv(0, 0);
    if(knockBack != 0.f) {
        k = cpvnormalize(op->getPosition() - body->p) * knockBack;
    }

    if((xDist * xDist) + (yDist * yDist) < r2)
        op->damage(d, s, speed, k);
}

void Ninja::draw(sf::RenderWindow *window, sf::Transform trans)
{
    cpVect pos = cpBodyGetPos(body);

    switch(state) {
        case WALKING:
            walkSprite.setRotation((dir * 45) - 180);
            walkSprite.setPosition(pos.x, pos.y);
            window->draw(walkSprite, trans);
            break;
        case DASHING:
            dashSprite.setRotation((dir * 45) - 180);
            dashSprite.setPosition(pos.x, pos.y);
            window->draw(dashSprite, trans);
            break;
        case ATTACKING:
            attackSprite.setRotation((dir * 45) - 180);
            attackSprite.setPosition(pos.x, pos.y);
            window->draw(attackSprite, trans);
            break;
        case ULT:
            ultSprite.setRotation((dir * 45) - 180);
            ultSprite.setPosition(pos.x, pos.y);
            window->draw(ultSprite, trans);
            break;
        case STANDING:
        default:
            walkSprite.setRotation((dir * 45) - 180);
            walkSprite.setPosition(pos.x, pos.y);
            window->draw(walkSprite, trans);
            break;
    }

    Character::drawArrow(window, trans);
}
