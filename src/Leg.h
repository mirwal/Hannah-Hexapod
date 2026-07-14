// Leg.h Hexapod ──◆ Leg
#pragma once
/**
 * Beschreibt ein einzelnes Bein des Hexapods.
 *
 * Die Klasse hält alle beinbezogenen Daten zusammen, wie Servo-IDs,
 * Offsets, Montageposition, Home-Position, aktuelle Position,
 * Zielposition und den aktuellen Bein-Zustand.
 *
 * Leg führt selbst keine Bewegungsberechnung aus und spricht keine Hardware an.
 * Die Klasse dient als geordneter Datensatz für ein Bein, der vom Hexapod
 * gefüllt, abgefragt und verwaltet wird.
 */
#include <cstdint>
#include "Vector3D.h"
#include "HexapodTypes.h"

struct JointOffsets
{
    float coxaOffset = 0.0f;
    float femurOffset = 0.0f;
    float tibiaOffset = 0.0f;
};

struct LegCalibration
{
    JointOffsets offsets;

    LegPositionLocal homePosition;   // neutrale Fußposition relativ zum Coxa-Montagepunkt
    MountPositionBody mountPosition; // Montagepunkt der Coxa im Body-Koordinatensystem
};

enum class LegState
{
    Idle,
    Swing,
    SearchingGround,
    LoadTransfer,
    Support
};

class Leg
{
public:
    Leg(uint8_t coxaServoID, uint8_t femurServoID, uint8_t tibiaServoID);

    void applyCalibration(const LegCalibration &calibration);

    uint8_t getServoID_Coxa() const { return coxaServoID; }
    uint8_t getServoID_Femur() const { return femurServoID; }
    uint8_t getServoID_Tibia() const { return tibiaServoID; }

    const JointOffsets &getOffsets() const { return offsets; }

    void setTargetPosition(const LegPositionLocal &position) { targetPosition = position; }
    const LegPositionLocal &getTargetPosition() const { return targetPosition; }
    void setCurrentPosition(const LegPositionLocal &position) { currentPosition = position; }
    const LegPositionLocal &getCurrentPosition() const { return currentPosition; }
    const LegPositionLocal &getHomePosition() const { return homePosition; }
    const MountPositionBody &getMountPosition() const { return mountPosition; }

    void setState(LegState newState) { state = newState; }
    LegState getState() const { return state; }

    void setOffsets(float coxaOff, float femurOff, float tibiaOff);

private:
    uint8_t coxaServoID = 0;
    uint8_t femurServoID = 0;
    uint8_t tibiaServoID = 0;

    JointOffsets offsets;

    LegPositionLocal homePosition;    // Wo steht der Fuß lokal neutral?
    LegPositionLocal targetPosition;  // Wo soll der Fuß hin relativ zum Coxa-Montagepunkt?
    LegPositionLocal currentPosition; // wo steht der Fuß aktuell relativ zum Coxa-Montagepunkt?
    MountPositionBody mountPosition;  // Wo sitzt dieses Bein am Body?

    LegState state = LegState::Idle;
};
