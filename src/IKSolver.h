// IKSolver.h
#pragma once
/**
 * Gelenkwinkel für ein einzelnes Bein.
 *
 * Enthält die berechneten Winkel für Coxa, Femur und Tibia.
 */
#include "HexapodTypes.h"
#include "HexapodGeometry.h"

struct IKDebugData
{
    float horizontalDistance = 0.0f;
    float legX = 0.0f;
    float legZ = 0.0f;
    float distance = 0.0f;
    float femurCos = 0.0f;
    float tibiaCos = 0.0f;
    float femurInnerAngle = 0.0f;
    float tibiaInnerAngle = 0.0f;
};

struct JointAngles
{
    float coxa = 0.0f;
    float femur = 0.0f;
    float tibia = 0.0f;
};
struct IKResult
{
    bool reachable = false;
    JointAngles angles;
    IKDebugData debug;
};

/**
 * Berechnet aus einer Bein-Zielposition die benötigten Gelenkwinkel.
 *
 * Der IKSolver kennt keine Servos, keine Servo-IDs und keine Gangart.
 * Er erhält eine Position im Bein-Koordinatensystem und liefert die
 * mathematisch berechneten Winkel für Coxa, Femur und Tibia zurück.
 */

class IKSolver
{
public:
    IKResult solve(const LegPositionLocal &targetPosition) const;

private:
    //  mutable IKDebugData debugData; MÜLL
    //  IKDebugData debugData;  MÜLL
};
