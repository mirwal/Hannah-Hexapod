// GaitController.cpp
#include "GaitController.h"
#include "ServoConfig.h"

void GaitController::update(float deltaTime)
{
    for (uint8_t i = 0; i < LEG_COUNT; i++)
    {
        footTargetsBody[i] = localToBody(i, FOOT_HOME_POS_LOCAL_MM);

        // test: joystick verschiebt alle Füße gleich
        footTargetsBody[i].x += gaitRequest.moveVecBody.x * 20.0f;
        footTargetsBody[i].y += gaitRequest.moveVecBody.y * 20.0f;
        footTargetsBody[i].z += gaitRequest.moveVecBody.z * 20.0f;
    }
}
void GaitController::updateSingleLegTest()
{
    static SingleLegStepPhase phase = SingleLegStepPhase::Home;
    static uint32_t phaseStartTime = 0;
    static float phaseProgress = 0.0f;
    static uint32_t phaseProgressCount = 0;
    static float nextDebugProgress = 0.0f;

    const uint32_t now = millis();

    MoveVectorBody stepVecBody =
        gaitRequest.moveVecBody * TEST_STEP_LENGTH_MM;

    LegPositionLocal stepVecLocal =
        rotateAroundZ(stepVecBody, -COXA_MOUNT_ANGLE_BODY_RAD[TEST_LEG_INDEX]);

    if (phaseStartTime == 0)
    {
        phaseStartTime = now;
    }

    uint32_t elapsed = now - phaseStartTime;
    phaseProgress = (float)elapsed / (float)TEST_PHASE_TIME_MS;

    // Alle Beine erstmal auf Home setzen und drehen, dass sie der Bewegungsrichtung folgen
    for (uint8_t i = 0; i < LEG_COUNT; i++)
    {
        footTargetsBody[i] = localToBody(i, FOOT_HOME_POS_LOCAL_MM);
    }

    if (phaseProgress >= 1.0f)
    {
        phaseStartTime = now;
        phaseProgress = 0.0f;
        nextDebugProgress = 0.0f;
        Serial.print("phaseProgress counter: ");
        Serial.println(++phaseProgressCount);

        switch (phase)
        {
        case SingleLegStepPhase::Home:
        {
            phase = SingleLegStepPhase::Lift;

            MoveVectorBody stepVecBody =
                gaitRequest.moveVecBody * TEST_STEP_LENGTH_MM;

            stepVecLocal =
                rotateAroundZ(stepVecBody, -COXA_MOUNT_ANGLE_BODY_RAD[TEST_LEG_INDEX]);

            Serial.print("Lift\n");

            break;
        }
        case SingleLegStepPhase::Lift:
        {
            phase = SingleLegStepPhase::MoveForward;
            Serial.print("MoveForward\n");
        }
        break;

        case SingleLegStepPhase::MoveForward:
        {
            phase = SingleLegStepPhase::Lower;
            Serial.print("Lower\n");
        }
        break;

        case SingleLegStepPhase::Lower:
        {
            phase = SingleLegStepPhase::ReturnHome;
            Serial.print("ReturnHome\n");
        }
        break;

        case SingleLegStepPhase::ReturnHome:
        {
            phase = SingleLegStepPhase::Done;
            Serial.print("Done\n");
        }
        break;

        case SingleLegStepPhase::Done:
        {
            phase = SingleLegStepPhase::Home;
            Serial.print("Home\n");
        }
        break;
        }
    }

    if (phaseProgress > 1.0f)
    {
        phaseProgress = 1.0f;
    }

    LegPositionLocal home = FOOT_HOME_POS_LOCAL_MM;
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
        end.x += stepVecLocal.x;
        end.y += stepVecLocal.y;

        break;
    }

    case SingleLegStepPhase::Lower:
    {
        start = home;
        start.z += TEST_LIFT_HEIGHT_MM;
        start.x += stepVecLocal.x;
        start.y += stepVecLocal.y;

        end = home;
        end.x += stepVecLocal.x;
        end.y += stepVecLocal.y;
        break;
    }

    case SingleLegStepPhase::ReturnHome:
    {
        start = home;
        start.x += stepVecLocal.x;
        start.y += stepVecLocal.y;

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

    //!!!!!!!!!!! DDDDEEEEEEBBBBUUUUUGGGGGG

    if (phaseProgress >= nextDebugProgress)
    {
        Serial.printf("phaseProgress=%.2f\n", phaseProgress);
        Serial.printf("MoveForward: start x=%.2f y=%.2f z=%.2f\n", start.x, start.y, start.z);
        Serial.printf("MoveForward: end x=%.2f y=%.2f z=%.2f\n", end.x, end.y, end.z);
        nextDebugProgress += 0.2f;
    }

    //!!!!!!!!!!! DDDDEEEEEEBBBBUUUUUGGGGGG
}
FootPositionBody GaitController::localToBody(uint8_t legIndex, const LegPositionLocal &local) const
{
    Vector3D rotated = rotateAroundZ(local, COXA_MOUNT_ANGLE_BODY_RAD[legIndex]);
    return COXA_MOUNT_POS_BODY_MM[legIndex] + rotated;
}
