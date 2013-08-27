#include "Fighter.h"

Fighter::Fighter()
{
    type = FIGHTER;
}

void Fighter::Init(cpSpace *space, float x, float y, Direction d, sf::Color c)
{
    texture = new sf::Texture();
    if(!texture->loadFromFile("rec/fighter.png")) {
        fprintf(stderr, "could not load 'walking.png'\n");
        return;
    }

    if(!buffer.loadFromFile("rec/punch.wav")) {
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

    attackSprite[0].setTexture(*texture);
    attackSprite[0].setTextureRect(sf::IntRect(1408, 0, 128, 128));
    attackSprite[0].setOrigin(64, 64);
    attackSprite[1].setTexture(*texture);
    attackSprite[1].setTextureRect(sf::IntRect(1536, 0, 128, 128));
    attackSprite[1].setOrigin(64, 64);

    ulting.setSpriteSheet(*texture);
    for(int i = 0; i < 13; i++)
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
    attacks = 0;
    attackFrames = 0;
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
    attackClock.restart();
    frameClock.restart();
}

Fighter::~Fighter()
{
}

void Fighter::dash()
{
    if(dashFrames <= 0 && stamina >= 50) {
        switch(state) {
            case STANDING:
                //standSprite.stop();
                break;
            case WALKING:
                walkSprite.stop();
                break;
            case ATTACKING:
                cpConstraintSetMaxForce(pivot, 1200.0f); // reset linear friction
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

void Fighter::attack()
{
    if(state == ULT) {
        if(ultLevel == 0 && ultClock.getElapsedTime() > sf::milliseconds(120))
            ultLevel++;
        else if(ultLevel == 1 && ultClock.getElapsedTime() > sf::milliseconds(480))
            ultLevel++;
    } else if(attackClock.getElapsedTime() >= sf::milliseconds(200) ||
            (attacks == 1 && attackClock.getElapsedTime() >= sf::milliseconds(100))) {
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
        cpConstraintSetMaxForce(pivot, 300.0f); // reduce linear friction

        cpBodySetVel(body, body->v + vel);

        state = ATTACKING;
        if(attackClock.getElapsedTime() >= sf::milliseconds(200))
            attacks = 0;

        checkDamage(900.f, 5, 50, 0.f, 0.f);
        attackClock.restart();
    }
}

void Fighter::move(Direction d)
{
    switch(state) {
        case STANDING:
            //standSprite.stop();
            break;
        case WALKING:
            if(dir == d) return;
            break;
        case ATTACKING:
            if(dir != d) {
                cpConstraintSetMaxForce(pivot, 1200.0f); // reset linear friction
            } else return;
            break;
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

void Fighter::ult()
{
    switch(state) {
        case WALKING:
            walkSprite.stop();
            break;
        case DASHING:
            dashFrames = 0;
            dashSprite.stop();
        case ATTACKING:
        case STANDING:
        default:
            break;
    }          

    cpConstraintSetMaxForce(pivot, 300.0f); // reduce linear friction
    state = ULT;

    cpVect vel = baseVelocity() * maxRunVelocity;
    vel = cpvclamp(vel, maxRunVelocity);
    cpBodySetVel(body, vel);

    ultSprite.play();
    ultClock.restart();
}

void Fighter::update()
{
    if(dashFrames > 0)
        dashFrames--;

    switch(state) {
        case WALKING:
            walkSprite.update(frameClock.restart());
            break;
        case ATTACKING:
            if(attackClock.getElapsedTime() >= sf::milliseconds(100)) {
                attacks = (attacks + 1) % 2;
                attackClock.restart();
                cpConstraintSetMaxForce(pivot, 1200.0f); // reset linear friction
                state = STANDING;
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
                    cpConstraintSetMaxForce(pivot, 1200.0f); // reset linear friction
                    ultToggle = false;
                }

                if(frame == 2 && !ultToggle) {
                    checkDamage(3600.f, 30, 60, -0.5, 1024.f);
                    ultToggle = true;
                } else if(frame == 4 && ultToggle) {
                    if(stamina >= 30) {
                        checkDamage(3600.f, 30, 60, -0.5, 1024.f);
                        stamina -= 30;
                    } else ultLevel--;
                    ultToggle = false;
                } else if(frame == 10 && !ultToggle) {
                    if(stamina >= 30) {
                        checkDamage(3600.f, 50, 75, -0.5, 1024.f);
                        stamina -= 30;
                    } else ultLevel--;
                    ultToggle = true;
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

void Fighter::checkDamage(float r2, int d, int s, float speed, float knockBack)
{
    float x = 0.f, y = 0.f;
    switch(dir) {
        case LEFT:
            x = -15.f;
            break;
        case UP_LEFT:
            x = -10.f;
            y = -10.f;
            break;
        case UP:
            y = -15.f;
            break;
        case UP_RIGHT:
            x = 10.f;
            y = -10.f;
            break;
        case RIGHT:
            x = 15.f;
            break;
        case DOWN_RIGHT:
            x = 10.f;
            y = 10.f;
            break;
        case DOWN:
            y = 15.f;
            break;
        case DOWN_LEFT:
            x = -10.f;
            y = 10.f;
            break;
    }

    cpVect damageArea = body->p + cpv(x, y);
    float xDist = damageArea.x - op->getPosition().x;
    float yDist = damageArea.y - op->getPosition().y;

    shoot.play();

    cpVect k = cpv(0, 0);
    if(knockBack != 0.f) {
        k = cpvnormalize(cpv(x, y));
        k = k * knockBack;
    }

    if((xDist * xDist) + (yDist * yDist) < r2)
        op->damage(d, s, speed, k);
}

void Fighter::draw(sf::RenderWindow *window, sf::Transform trans)
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
            attackSprite[attacks].setRotation((dir * 45) - 180);
            attackSprite[attacks].setPosition(pos.x, pos.y);
            window->draw(attackSprite[attacks], trans);
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
