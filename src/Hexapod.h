// Hexapod.h
#pragma once

/**
 * Zentrale Steuerklasse für den sechbeinigen Roboter HANNAH.
 *
 * Die Hexapod-Klasse ist der Dirigent des Systems. Sie wertet die
 * aufbereiteten Steuerdaten aus, verwaltet den internen Zustand von HANNAH
 * und koordiniert Leg, GaitController, IKSolver und ServoBus.
 *
 * Hexapod berechnet die Gangart nicht selbst, löst die inverse Kinematik
 * nicht selbst und spricht die Dynamixel-Servos nicht direkt auf Registerebene an.
 * Stattdessen verteilt sie die Aufgaben an die zuständigen Klassen.
 *
 * Hexapod ───> ServoBus
 * → benutzt den ServoBus, besitzt ihn aber nicht.
 *
 * Hexapod ──◆ Leg[6]
 * Hexapod ──◆ IKSolver
 * Hexapod ──◆ GaitController
 * → gehören fest zum Hexapod.
 *
 * Die Klasse ist unabhängig von der Funksteuerung. Sie arbeitet nur mit
 * HexapodControl und kann dadurch später auch von anderen Eingabequellen
 * gesteuert werden.
 */
#include <Arduino.h>
#include <cstdint>

#include "HexapodGeometry.h"
#include "HexapodControl.h"
#include "Leg.h"
#include "IKSolver.h"
#include "ServoBus.h"
#include "GaitController.h"
#include "SystemStatus.h"

enum class HexapodStateMode : uint8_t // tatsächlicher interner Zustand von HANNAH
{
    Idle,
    Initializing,
    Standing,
    Walking,
    Posing,
    Testing,
    Error
};

class Hexapod
{
public:
    explicit Hexapod(ServoBus &bus, SystemStatus &status) : servoBus(bus), systemStatus(status) {};

    void begin();
    void update(const HexapodControl &control);

    void setState(HexapodStateMode newState) { state = newState; }
    HexapodStateMode getState() const { return state; }
    void applyAnglesToLeg(uint8_t legIndex, const JointAngles &angles);
    LegPositionLocal transformBodyToLegLocal(uint8_t legIndex, const FootPositionBody &footTargetBody) const;
    void setDebugMode(HexapodDebugMode mode) { debugMode = mode; }
    HexapodDebugMode getDebugMode() const { return debugMode; }
    String createServoIdReport() const;
    void debugRoundtripTest();

private:
    ServoBus &servoBus;
    SystemStatus &systemStatus;

    Leg legs[LEG_COUNT] = {{1, 7, 13}, {2, 8, 14}, {3, 9, 15}, {4, 10, 16}, {5, 11, 17}, {6, 12, 18}};

    IKSolver ikSolver;
    GaitController gaitController;

    HexapodStateMode state = HexapodStateMode::Idle;
    HexapodDebugMode debugMode = HexapodDebugMode::Off;
    bool keyboardControlEnabled = false;
    void updateIdle();
    void updateError();
    void updateWalking(const HexapodControl &control);
    void updatePosing(const HexapodControl &control);
    void updateTesting(const HexapodControl &control);
    void applyToServos();
};