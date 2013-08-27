#include "Shooter.h"

Shooter::Shooter()
{
    type = SHOOTER;
}

void Shooter::Init(cpSpace *space, float x, float y, Direction d, sf::Color c)
{
    texture = new sf::Texture();
    if(!texture->loadFromFile("rec/shock.png")) {
        fprintf(stderr, "could not load 'walking.png'\n");
        return;
    }

    laserball = new sf::Texture();
    if(!laserball->loadFromFile("rec/laserball.png")) {
        fprintf(stderr, "could not load file 'laserball.png'\n");
        return;
    }

    ultraTexture = new sf::Texture();
    if(!ultraTexture->loadFromFile("rec/ultralaser.png")) {
        fprintf(stderr, "could not load file 'ultralaser/png'\n");
        return;
    }

    if(!buffer.loadFromFile("rec/shoot.wav")) {
        fprintf(stderr, "could not load file 'shoot.wav'\n");
    }
    shoot.setBuffer(buffer);
    shoot.setVolume(50);

    if(!lbuffer.loadFromFile("rec/laser.wav")) {
        fprintf(stderr, "could not load file 'laser.wav'\n");
    }
    laser.setBuffer(lbuffer);
    laser.setVolume(60);
    playLaser = true;

    ultraSprite.setTexture(*ultraTexture);
    ultraSprite.setOrigin(0, 64);

    arrowColor = c;
    dir = d;

    texture->setSmooth(true);

    walking.setSpriteSheet(*texture);
    for(int i = 0; i < 8; i++)
        walking.addFrame(sf::IntRect(128 * i, 128, 128, 128));

    walkSprite.setFrameTime(sf::seconds(0.09));
    walkSprite.setAnimation(walking);
    walkSprite.setOrigin(64, 64);

    dashing.setSpriteSheet(*texture);
    for(int i = 0; i < 3; i++)
        dashing.addFrame(sf::IntRect((128 * i) + 1024, 128, 128, 128));

    dashSprite.setFrameTime(sf::seconds(0.12));
    dashSprite.setAnimation(dashing);
    dashSprite.setOrigin(64, 64);

    attacking.setSpriteSheet(*texture);
    for(int i = 0; i < 3; i++)
        attacking.addFrame(sf::IntRect((128 * i) + 1408, 128, 128, 128));

    attackSprite.setFrameTime(sf::seconds(0.09));
    attackSprite.setAnimation(attacking);
    attackSprite.setOrigin(64, 64);
    attackSprite.setLooped(false);
    attackSprite.stop();

    ulting.setSpriteSheet(*texture);
    for(int i = 0; i < 16; i++)
        ulting.addFrame(sf::IntRect(128 * i, 0, 128, 128));

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

    ultClock.restart();

    staminaTime = sf::milliseconds(0);
    staminaFillTimer.restart();
    frameClock.restart();
}

Shooter::~Shooter()
{
}

void Shooter::ReInit(float x, float y, Direction d)
{
    Character::ReInit(x, y, d);
    for(int i = 0; i < bullets.size(); i++) {
        if(bullets.at(i)->destroyed) {
            delete bullets.at(i);
            bullets.erase(bullets.begin() + i);
            continue;
        }
    }
}

void Shooter::dash()
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

void Shooter::attack()
{
    if(state == ULT) {
        if(ultLevel == 0 && ultClock.getElapsedTime() > sf::milliseconds(240))
            ultLevel++;
        else if(ultLevel == 1 && ultClock.getElapsedTime() > sf::milliseconds(1080))
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
        cpConstraintSetMaxForce(pivot, 300.0f); // reduce linear friction

        cpBodySetVel(body, body->v - vel);

        attackSprite.play();
        state = ATTACKING;
        fireBullet();
    }
}

void Shooter::move(Direction d)
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

void Shooter::ult()
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

    cpConstraintSetMaxForce(pivot, 1200.0f); // reset linear friction
    state = ULT;

    ultSprite.play();
    ultClock.restart();
}

void Shooter::update()
{
    if(dashFrames > 0)
        dashFrames--;

    sf::Time t = frameClock.restart();
    switch(state) {
        case STANDING:
            break;
        case WALKING:
            walkSprite.update(t);
            break;
        case ATTACKING:
            attackSprite.update(t);
            if(!attackSprite.isPlaying()) {
                cpConstraintSetMaxForce(pivot, 1200.0f); // reset linear friction
                state = STANDING;
            }
            break;
        case DASHING:
            dashSprite.update(t);
            if(dashFrames <= 10) {
                state = STANDING;
                dashSprite.stop();
            }
            break;
        case ULT:
            {
                cpBodySetVel(body, cpv(0.f, 0.f));
                checkUltCollisions();
                ultSprite.update(t);

                int frame = ultSprite.getFrame();
                sf::Time t = ultClock.getElapsedTime();
                if(t > sf::seconds(1.92) || (frame == 12 && ultLevel < 2)
                        || (frame == 8 && ultLevel < 1)) {
                    state = STANDING;
                    ultSprite.stop();
                    ultLevel = 0;
                    ultToggle = false;
                    playLaser = true;
                    break;
                }
                if(frame == 9 && !ultToggle) {
                    fireUlt1();
                    ultToggle = true;
                } else if(frame == 11 && ultToggle) {
                    if(stamina >= 30) {
                        fireUlt1();
                        stamina -= 30;
                    } else ultLevel--;
                    ultToggle = false;
                } else if(frame == 14 && !ultToggle) {
                    if(stamina >= 30) {
                        fireUlt2();
                        stamina -= 30;
                    } else ultLevel--;
                    ultToggle = true;
                }
            }
        default:
            break;
    } 

    staminaTime += staminaFillTimer.restart();
    if(staminaTime.asMilliseconds() > 40) {
        if(stamina < 100) stamina++;
        staminaTime -= sf::milliseconds(40);
    }

    for(int i = 0; i < bullets.size(); i++) {
        if(bullets.at(i)->destroyed) {
            delete bullets.at(i);
            bullets.erase(bullets.begin() + i);
            continue;
        }

        bullets.at(i)->update(t);
        if(bullets.at(i)->exploding) continue;

        checkBulletCollision(bullets.at(i), op);
    }
} 

void Shooter::checkBulletCollision(Bullet *b, Character *c)
{
    cpVect bp = b->getPosition();
    cpVect cp = c->getPosition();

    float x = bp.x - cp.x;
    float y = bp.y - cp.y;
    if((x * x) + (y * y) < b->getR2()) {
        b->explode();
        cpVect vel = b->getVelocity();
        vel = vel * 0.15f;

        int s = 0;
        if(b->ult)
            s = -.5;

        int d = 5;
        if(b->ult)
            d = 10;
        if(b->getR2() > 4000)
            d = 25;

        c->damage(d, 40, s, vel);
    }

    if(bp.x < 190 || bp.x > 1090 || bp.y < 35 || bp.y > 685)
        b->explode();
}

void Shooter::fireUlt1()
{
    shoot.play();
    cpVect p = body->p + firePos();
    cpVect v = baseVelocity() * 20;
    bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x, v.y, (dir * 45) - 180, laserball, true));
    bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x , v.y, (dir * 45) - 180, laserball, true));
    bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x, v.y, (dir * 45) - 180, laserball, true));

    switch(dir) {
        case LEFT:
            bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x + 2, v.y - 6, (dir * 45) - 180, laserball, true));
            bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x + 2, v.y + 6, (dir * 45) - 180, laserball, true));
            break;
        case UP_LEFT:
            bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x + 4, v.y - 2, (dir * 45) - 180, laserball, true));
            bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x - 2, v.y + 4, (dir * 45) - 180, laserball, true));
            break;
        case UP:
            bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x - 6, v.y + 2, (dir * 45) - 180, laserball, true));
            bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x + 6, v.y + 2, (dir * 45) - 180, laserball, true));
            break;
        case UP_RIGHT:
            bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x + 2, v.y + 4, (dir * 45) - 180, laserball, true));
            bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x - 4, v.y - 2, (dir * 45) - 180, laserball, true));
            break;
        case RIGHT:
            bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x - 2, v.y - 6, (dir * 45) - 180, laserball, true));
            bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x - 2, v.y + 6, (dir * 45) - 180, laserball, true));
            break;
        case DOWN_RIGHT:
            bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x + 2, v.y - 4, (dir * 45) - 180, laserball, true));
            bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x - 4, v.y + 2, (dir * 45) - 180, laserball, true));
            break;
        case DOWN:
            bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x + 6, v.y - 2, (dir * 45) - 180, laserball, true));
            bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x - 6, v.y - 2, (dir * 45) - 180, laserball, true));
            break;
        case DOWN_LEFT:
            bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x + 4, v.y + 2, (dir * 45) - 180, laserball, true));
            bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x - 2, v.y - 4, (dir * 45) - 180, laserball, true));
            break;
    }
}

void Shooter::fireUlt2()
{
    shoot.play();
    cpVect p = body->p + firePos();
    cpVect v = baseVelocity() * 20;
    bullets.push_back(new Bullet(p.x, p.y, 4048.f, v.x, v.y, (dir * 45) - 180, laserball, true));
}

void Shooter::checkUltCollisions()
{
    if(ultClock.getElapsedTime() < sf::seconds(0.36)) return;
    if(playLaser) {
        laser.play();
        playLaser = false;
    }

    cpVect line = firePos() * 128;
    cpVect posOff = body->p - op->getPosition();
    float a = cpvdot(line, line);
    float b = 2 * cpvdot(posOff, line);
    float c = cpvdot(posOff, posOff) - 900;

    float discriminant = b * b - (4 * a * c);
    if(discriminant < 0) return;

    discriminant = sqrt(discriminant);

    float t1 = (-b - discriminant) / (2 * a);
    float t2 = (-b + discriminant) / (2 * a);

    if((t1 >= 0 && t1 <= 1) || (t2 >= 0 && t2 <= 1)) {
        cpVect k = cpvnormalize(firePos()) * 24.f ;
        op->damage(0.5f, 2, -0.75f, k);
    }
}

cpVect Shooter::firePos()
{
    float x = 0.f, y = 0.f;
    switch(dir) {
        case LEFT:
            x = -40.f;
            break;
        case UP_LEFT:
            x = -24.f;
            y = -24.f;
            break;
        case UP:
            y = -40.f;
            break;
        case UP_RIGHT:
            x = 24.f;
            y = -24.f;
            break;
        case RIGHT:
            x = 40.f;
            break;
        case DOWN_RIGHT:
            x = 24.f;
            y = 24.f;
            break;
        case DOWN:
            y = 40.f;
            break;
        case DOWN_LEFT:
            x = -24.f;
            y = 24.f;
            break;
    }
    return cpv(x, y);     
}

void Shooter::fireBullet()
{
    shoot.play();
    cpVect p = body->p + firePos();
    cpVect v = baseVelocity() * 20;
    bullets.push_back(new Bullet(p.x, p.y, 1200.f, v.x, v.y, (dir * 45) - 180, laserball, false));
}

void Shooter::draw(sf::RenderWindow *window, sf::Transform trans)
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
            if(ultClock.getElapsedTime() > sf::seconds(0.36)) {
                cpVect pos = body->p + firePos();
                ultraSprite.setPosition(pos.x, pos.y);
                ultraSprite.setRotation((dir * 45) - 180);
                window->draw(ultraSprite, trans);
            }
            break;
        case STANDING:
        default:
            walkSprite.setRotation((dir * 45) - 180);
            walkSprite.setPosition(pos.x, pos.y);
            window->draw(walkSprite, trans);
            break;
    }

    drawArrow(window, trans);
}

void Shooter::drawBullets(sf::RenderWindow *window, sf::Transform trans)
{
    for(Bullet *b : bullets)
        b->draw(window, trans);
}
