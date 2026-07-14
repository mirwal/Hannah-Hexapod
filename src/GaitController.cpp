#include "GaitController.h"

void GaitController::update(float deltaTime)
{
    for (uint8_t i = 0; i < LEG_COUNT; i++)
    {
        footTargetsBody[i] = mountPos[i] + footHomeOffsetBody[i];

        // test: joystick verschiebt alle Füße gleich
        footTargetsBody[i].x += gaitRequest.dirX * 20.0f;
        footTargetsBody[i].y += gaitRequest.dirY * 20.0f;
        footTargetsBody[i].z += gaitRequest.rot * 20.0f;
    }
}