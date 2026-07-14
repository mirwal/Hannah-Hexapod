// Hexapod.cpp
#include "Hexapod.h"
#include "GaitController.h"

// Testparameter für die Einzelbeinbewegung
inline constexpr uint8_t TEST_LEG_INDEX = 0;

inline constexpr float TEST_LIFT_HEIGHT_MM = 20.0f;
inline constexpr float TEST_STEP_FORWARD_MM = 30.0f;
inline constexpr uint32_t TEST_PHASE_TIME_MS = 1000;

void Hexapod::updateTesting(const HexapodControl &control)
{
    static SingleLegStepPhase phase = SingleLegStepPhase::Home;
    static uint32_t phaseStartTime = 0;
    static float phaseProgress = 0.0f;
    const uint32_t now = millis();

    if (phaseStartTime == 0)
    {
        phaseStartTime = now;
    }

    if (phaseProgress >= 1.0f)
    {
        phaseStartTime = now;
        phaseProgress = 0.0f;
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

    LegPositionLocal home = legs[TEST_LEG_INDEX].getHomePosition();
    LegPositionLocal start = home;
    LegPositionLocal end = home;
    LegPositionLocal target = home;

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

    target = interpolate(start, end, phaseProgress);

    IKResult result = ikSolver.solve(target);

    if (!result.reachable)
    {
        systemStatus.setFlag(SystemFlag::IkNotReachable);
        return;
    }

    applyAnglesToLeg(TEST_LEG_INDEX, result.angles);

    uint32_t elapsed = now - phaseStartTime;
    phaseProgress = (float)elapsed / (float)TEST_PHASE_TIME_MS;

    if (phaseProgress > 1.0f)
    {
        phaseProgress = 1.0f;
    }

    if (control.debugMode == HexapodDebugMode::Maximal)
    {
        Serial.printf("Phase=%u progress=%.2f | Target x=%.2f y=%.2f z=%.2f\n",
                      static_cast<uint8_t>(phase),
                      phaseProgress,
                      target.x,
                      target.y,
                      target.z);
    }
}

void Hexapod::begin()
{
    for (uint8_t i = 0; i < LEG_COUNT; i++)
    {
        LegCalibration calibration;

        calibration.mountPosition = mountPos[i];

        // vorläufig: lokales Home für alle Beine gleich
        calibration.homePosition = {AF_CA, 0.0f, -80.0f};

        calibration.offsets = {0.0f, 0.0f, 10.0f};

        legs[i].applyCalibration(calibration);
    }
}

void Hexapod::update(const HexapodControl &control)
{
    debugMode = control.debugMode;

    if (!control.online)
    {
        state = HexapodStateMode::Idle;
        systemStatus.setFlag(SystemFlag::FunkOffline);
        return;
    }

    if (control.resetStatusRequested)
    {
        Serial.print("SystemFlags BIN: ");
        Serial.println(systemStatus.getFlags(), BIN);

        Serial.print("SystemFlags HEX: 0x");
        Serial.println(systemStatus.getFlags(), HEX);

        systemStatus.clear();
    }

    switch (control.moveMode)
    {
    case HexapodMoveMode::Testing:
        state = HexapodStateMode::Testing;
        updateTesting(control);
        break;

    default:
        state = HexapodStateMode::Idle;
        updateIdle();

        break;
    }
}

String Hexapod::createServoIdReport() const
{
    String result;
    uint8_t pingCount = 0;
    for (uint8_t i = 0; i < LEG_COUNT; i++)
    {

        uint8_t coxaID = legs[i].getServoID_Coxa();
        uint8_t femurID = legs[i].getServoID_Femur();
        uint8_t tibiaID = legs[i].getServoID_Tibia();
        result += "Leg ";
        result += String(i + 1);
        result += ": C ID ";
        (coxaID <= 9) ? (result += " ") : (result += "");

        if (servoBus.ping(coxaID))
        {
            pingCount++;
            result += String(coxaID);
        }
        else
        {
            (coxaID <= 9) ? (result += "█") : (result += " █");
        }

        result += ", F ID ";
        (femurID <= 9) ? (result += " ") : (result += "");

        if (servoBus.ping(femurID))
        {
            pingCount++;
            result += String(femurID);
        }
        else
        {
            (femurID <= 9) ? (result += "█") : (result += " █");
        }

        result += ", T ID ";
        (tibiaID <= 9) ? (result += " ") : (result += "");

        if (servoBus.ping(tibiaID))
        {
            pingCount++;
            result += String(tibiaID);
        }

        else
        {
            (tibiaID <= 9) ? (result += "█") : (result += " █");
        }
        result += "\n";
    }

    if ((pingCount) != static_cast<uint8_t>(LEG_COUNT * 3))
    {
        result += "--> ERR⌂R:  Nicht  alle  Servos  gefunden!\n";
        systemStatus.setFlag(SystemFlag::ServoNotFound);
    }

    return result;
}

// IDLE: Home-Pose
void Hexapod::updateIdle()
{
    // for (int i = 0; i < 6; ++i)
    // {
    //     float c, f, t;
    //     //  ik.compute(i, c, f, t, 0.0f, 0, 0, 0, 0);
    //     // legs[i].setTargetAngles(c, f, t);
    // }
}

void Hexapod::updateError()
{
    delay(5000);
}

void Hexapod::updateWalking(const HexapodControl &control)
{
}

void Hexapod::updatePosing(const HexapodControl &control)
{
}

void Hexapod::applyAnglesToLeg(uint8_t legIndex, const JointAngles &angles)
{
    const JointOffsets &offsets = legs[legIndex].getOffsets();

    float coxaServo =
        COXA_SERVO_CENTER_DEG +
        COXA_SERVO_DIR * angles.coxa +
        COXA_SERVO_OFFSET_DEG;

    float femurServo =
        FEMUR_SERVO_CENTER_DEG +
        FEMUR_SERVO_DIR * angles.femur +
        FEMUR_SERVO_OFFSET_DEG;

    float tibiaServo =
        TIBIA_SERVO_CENTER_DEG +
        TIBIA_SERVO_DIR * angles.tibia +
        TIBIA_SERVO_OFFSET_DEG;

    coxaServo = constrain(coxaServo, SERVO_MIN_DEG, SERVO_MAX_DEG);
    femurServo = constrain(femurServo, SERVO_MIN_DEG, SERVO_MAX_DEG);
    tibiaServo = constrain(tibiaServo, SERVO_MIN_DEG, SERVO_MAX_DEG);

    if (debugMode == HexapodDebugMode::Maximal)
    {
        Serial.printf("\tServo angles:\ncoxaServo=\t%.2f\nfemurServo=\t%.2f\ntibiaServo=\t%.2f\t\n-----------------------\n", coxaServo, femurServo, tibiaServo);

        Serial.println("\tnach constrain");
        Serial.printf("\tServo angles:\ncoxaServo=\t%.2f\nfemurServo=\t%.2f\ntibiaServo=\t%.2f\t\n-----------------------\n", coxaServo, femurServo, tibiaServo);
    }
    servoBus.setPosition(
        legs[legIndex].getServoID_Coxa(),
        coxaServo + offsets.coxaOffset);

    servoBus.setPosition(
        legs[legIndex].getServoID_Femur(),
        femurServo + offsets.femurOffset);

    servoBus.setPosition(
        legs[legIndex].getServoID_Tibia(),
        tibiaServo + offsets.tibiaOffset);

    // später: offsets in die Berechnung der Winkel einbeziehen, damit die Servos korrekt positioniert werden
    // servoBus.setPosition(coxaServo,  SERVO_CENTER_DEG + angles.coxa);
    // servoBus.setPosition(femurServo, SERVO_CENTER_DEG - angles.femur);
    // servoBus.setPosition(tibiaServo, SERVO_CENTER_DEG + angles.tibia);
}

LegPositionLocal Hexapod::transformBodyToLegLocal(uint8_t legIndex, const FootPositionBody &footTargetBody) const
{
    MountPositionBody mount = legs[legIndex].getMountPosition();

    Vector3D offsetBody = footTargetBody - mount;

    float phi = mountPhi[legIndex];
    float c = cosf(-phi);
    float s = sinf(-phi);

    return {
        offsetBody.x * c - offsetBody.y * s,
        offsetBody.x * s + offsetBody.y * c,
        offsetBody.z};
}

void Hexapod::applyToServos()
{
    // for (auto &leg : legs)
    // {
    //     float c, f, t;
    //     //       leg.getTargetAngles(c, f, t);
    //     // servoBus.setPosition(leg.getID_Coxa(), c);
    //     // servoBus.setPosition(leg.getID_Femur(), f);
    //     // servoBus.setPosition(leg.getID_Tibia(), t);
    // }
}
