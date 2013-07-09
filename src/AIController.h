#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include "Character.h"

class AIController
{
    public:
        AIController() { };
        AIController(Character *c);

        void update();

    private:
        Character *character;

        enum AIState {
            IDLE, WONDERING, ATTACKING
        } state;
};

#endif
