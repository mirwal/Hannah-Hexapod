// Hexapod.cpp
#include "Hexapod.h"

void Hexapod::updateTesting(const HexapodControl &control)
{

    GaitRequest request;
    request.moveVecBody = control.moveDirBody;
    request.yaw = control.yaw;
    request.speed = 1.0f;
    request.pitch = control.pitch;

    gaitController.setGaitRequest(request);

    gaitController.updateSingleLegTest();

    FootPositionBody footTargetBody =
        gaitController.getFootTargetBody(TEST_LEG_INDEX);

    LegPositionLocal localTarget =
        transformBodyToLegLocal(TEST_LEG_INDEX, footTargetBody);

    IKResult result = ikSolver.solve(localTarget);

    if (!result.reachable)
    {
        systemStatus.setFlag(SystemFlag::IkNotReachable);
        return;
    }

    applyAnglesToLeg(TEST_LEG_INDEX, result.angles);

    if (control.debugMode == HexapodDebugMode::Maximal)
    {
        Serial.printf("Body Target \tx=%.2f \ty=%.2f \tz=%.2f\n",
                      footTargetBody.x,
                      footTargetBody.y,
                      footTargetBody.z);

        Serial.printf("Local Target \tx=%.2f \ty=%.2f \tz=%.2f\n",
                      localTarget.x,
                      localTarget.y,
                      localTarget.z);
        debugRoundtripTest();
    }
}

void Hexapod::begin()
{
    for (uint8_t i = 0; i < LEG_COUNT; i++)
    {
        LegCalibration calibration;

        calibration.mountPosition = COXA_MOUNT_POS_BODY_MM[i];

        // vorläufig: lokales Home für alle Beine gleich
        calibration.homePosition = FOOT_HOME_POS_LOCAL_MM;

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

void Hexapod::debugRoundtripTest()
{
    for (uint8_t i = 0; i < LEG_COUNT; i++)
    {
        FootPositionBody body =
            gaitController.localToBody(i, FOOT_HOME_POS_LOCAL_MM);

        LegPositionLocal local =
            transformBodyToLegLocal(i, body);

        Serial.printf("Leg %u Roundtrip local: x=%.2f y=%.2f z=%.2f\n",
                      i + 1,
                      local.x,
                      local.y,
                      local.z);
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

    float phi = COXA_MOUNT_ANGLE_BODY_RAD[legIndex];
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
