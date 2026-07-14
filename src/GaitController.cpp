// GaitController.cpp
#include "GaitController.h"

void GaitController::update(float deltaTime)
{
    for (uint8_t i = 0; i < LEG_COUNT; i++)
    {
        footTargetsBody[i] = localToBody(i, FOOT_HOME_LOCAL);

        // test: joystick verschiebt alle Füße gleich
        footTargetsBody[i].x += gaitRequest.dirX * 20.0f;
        footTargetsBody[i].y += gaitRequest.dirY * 20.0f;
        footTargetsBody[i].z += gaitRequest.rot * 20.0f;
    }
}

void GaitController::updateSingleLegTest()
{
    static SingleLegStepPhase phase = SingleLegStepPhase::Home;
    static uint32_t phaseStartTime = 0;
    static float phaseProgress = 0.0f;

    const uint32_t now = millis();

    if (phaseStartTime == 0)
    {
        phaseStartTime = now;
    }

    // Alle Beine erstmal auf Home setzen
    for (uint8_t i = 0; i < LEG_COUNT; i++)
    {
        footTargetsBody[i] = localToBody(i, FOOT_HOME_LOCAL);
    }

    uint32_t elapsed = now - phaseStartTime;
    phaseProgress = (float)elapsed / (float)TEST_PHASE_TIME_MS;

    if (phaseProgress >= 1.0f)
    {
        phaseProgress = 0.0f;
        phaseStartTime = now;

        switch (phase)
        {
        case SingleLegStepPhase::Home:
            phase = SingleLegStepPhase::Lift;
            break;

        case SingleLegStepPhase::Lift:
            phase = SingleLegStepPhase::MoveForward;
            break;

        case SingleLegStepPhase::MoveForward:
            phase = SingleLegStepPhase::Lower;
            break;

        case SingleLegStepPhase::Lower:
            phase = SingleLegStepPhase::ReturnHome;
            break;

        case SingleLegStepPhase::ReturnHome:
            phase = SingleLegStepPhase::Done;
            break;

        case SingleLegStepPhase::Done:
            phase = SingleLegStepPhase::Home;
            break;
        }
    }

    if (phaseProgress > 1.0f)
    {
        phaseProgress = 1.0f;
    }

    LegPositionLocal home = FOOT_HOME_LOCAL;
    LegPositionLocal start = home;
    LegPositionLocal end = home;

    switch (phase)
    {
    case SingleLegStepPhase::Home:
    {
        start = home;
        end = home;
        break;
    }

    case SingleLegStepPhase::Lift:
    {
        start = home;
        end = home;
        end.z += TEST_LIFT_HEIGHT_MM;
        break;
    }

    case SingleLegStepPhase::MoveForward:
    {
        start = home;
        start.z += TEST_LIFT_HEIGHT_MM;

        end = start;
        end.x += TEST_STEP_FORWARD_MM;
        break;
    }

    case SingleLegStepPhase::Lower:
    {
        start = home;
        start.z += TEST_LIFT_HEIGHT_MM;
        start.x += TEST_STEP_FORWARD_MM;

        end = home;
        end.x += TEST_STEP_FORWARD_MM;
        break;
    }

    case SingleLegStepPhase::ReturnHome:
    {
        start = home;
        start.x += TEST_STEP_FORWARD_MM;

        end = home;
        break;
    }

    case SingleLegStepPhase::Done:
    {
        start = home;
        end = home;
        break;
    }
    }

    LegPositionLocal targetLocal =
        interpolate(start, end, phaseProgress);

    footTargetsBody[TEST_LEG_INDEX] =
        localToBody(TEST_LEG_INDEX, targetLocal);
}

FootPositionBody GaitController::localToBody(uint8_t legIndex, const LegPositionLocal &local) const
{
    Vector3D rotated = rotateAroundZ(local, mountPhi[legIndex]);
    return mountPos[legIndex] + rotated;
}