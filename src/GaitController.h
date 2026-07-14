// GaitController.h
#pragma once
/**
 * Berechnet die Fuß-Zielpositionen für die Gangbewegung des Hexapods.
 *
 * Der GaitController erhält vom Hexapod die gewünschte Bewegungsrichtung
 * und Rotation und erzeugt daraus für jedes Bein eine Zielposition.
 * Er kennt keine Servo-IDs, keine Hardware und führt keine inverse Kinematik aus.
 */
#include <cstdint>
#include <Arduino.h>
#include "Vector3D.h"
#include "HexapodTypes.h"
#include "HexapodGeometry.h"

// test: Einzelbeinbewegung
inline constexpr uint8_t TEST_LEG_INDEX = 1;
inline constexpr float TEST_LIFT_HEIGHT_MM = 20.0f;
inline constexpr float TEST_STEP_FORWARD_MM = 30.0f;
inline constexpr uint32_t TEST_PHASE_TIME_MS = 1000;

enum class SingleLegStepPhase : uint8_t
{
    Home,
    Lift,
    MoveForward,
    Lower,
    ReturnHome,
    Done
};
enum class GaitType : uint8_t // Typ des Gangs
{
    Tripod,
    Ripple,
    Wave
};

struct GaitRequest // Bewegungsanfrage an den GaitController
{
    float dirX = 0.0f;
    float dirY = 0.0f;
    float rot = 0.0f;
    float speed = 1.0f;
    GaitType type = GaitType::Tripod;
};

class GaitController // Berechnet die Fuß-Zielpositionen für die Gangbewegung des Hexapods
{
public:
    void setGaitRequest(const GaitRequest &request) { gaitRequest = request; }                       // Eingabe der Bewegungsrichtung und Geschwindigkeit
    void update(float deltaTime);                                                                    // Berechnung der Zielpositionen für alle Beine basierend auf der Eingabe und phase
    void updateSingleLegTest();                                                                      // Test: Einzelbeinbewegung für das Bein TEST_LEG_INDEX
    FootPositionBody getFootTargetBody(uint8_t legIndex) const { return footTargetsBody[legIndex]; } // Zielposition im Körper-Koordinatensystem abfragen
    FootPositionBody localToBody(uint8_t legIndex, const LegPositionLocal &local) const;

private:
    float phase = 0.0f;
    GaitRequest gaitRequest;
    FootPositionBody footTargetsBody[LEG_COUNT];
};