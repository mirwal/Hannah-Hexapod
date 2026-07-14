// Leg.cpp
#include "Leg.h"

Leg::Leg(uint8_t coxaServoID, uint8_t femurServoID, uint8_t tibiaServoID)
    : coxaServoID(coxaServoID),
      femurServoID(femurServoID),
      tibiaServoID(tibiaServoID)
{
}

void Leg::applyCalibration(const LegCalibration &calibration)
{
    offsets = calibration.offsets;
    homePosition = calibration.homePosition;
    mountPosition = calibration.mountPosition;
    targetPosition = calibration.homePosition;
    currentPosition = calibration.homePosition;

    state = LegState::Idle;
}

void Leg::setOffsets(float coxaOff, float femurOff, float tibiaOff)
{
    offsets.coxaOffset = coxaOff;
    offsets.femurOffset = femurOff;
    offsets.tibiaOffset = tibiaOff;
}
