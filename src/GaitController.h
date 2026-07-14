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
#include "HexapodTypes.h"
#include "HexapodGeometry.h"
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
    void setGaitRequest(const GaitRequest &request) { gaitRequest = request; } // Eingabe der Bewegungsrichtung und Geschwindigkeit
    void update(float deltaTime);                                              // Berechnung der Zielpositionen für alle Beine basierend auf der Eingabe und phase

    FootPositionBody getFootTargetBody(uint8_t legIndex) const { return footTargetsBody[legIndex]; } // Zielposition im Körper-Koordinatensystem abfragen

private:
    float phase = 0.0f;
    GaitRequest gaitRequest;
    FootPositionBody footTargetsBody[LEG_COUNT];
};