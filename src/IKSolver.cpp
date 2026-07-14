// IKSolver.cpp
#include "IKSolver.h"
#include <Arduino.h>
// hier kann ich vergleichen https://www.geogebra.org/m/U4pTpDsr
IKResult IKSolver::solve(const LegPositionLocal &targetPosition) const
{
    IKResult result;

    // 1. Entfernung in der Horizontalebene
    float horizontalDistance = sqrtf(
        targetPosition.x * targetPosition.x +
        targetPosition.y * targetPosition.y);
    result.debug.horizontalDistance = horizontalDistance;

    // 2. Coxa-Länge abziehen: ab hier beginnt das Femur/Tibia-Dreieck
    float legX = horizontalDistance - COXA_LEN;
    float legZ = targetPosition.z;
    result.debug.legX = legX;
    result.debug.legZ = legZ;

    // 3. Abstand vom Femur-Gelenk zum Fußziel
    float distance = sqrtf(legX * legX + legZ * legZ);
    result.debug.distance = distance;

    // 4. Erreichbarkeit prüfen
    if (distance > FEMUR_LEN + TIBIA_LEN)
    {
        result.reachable = false;
        Serial.println(distance);
        return result;
    }

    if (distance < fabsf(FEMUR_LEN - TIBIA_LEN))
    {
        result.reachable = false;
        Serial.println(distance);
        return result;
    }

    // 5. Winkel zum Zielpunkt
    float baseAngle = atan2f(legZ, legX);

    // 6. Winkelanteil am Femur über Kosinussatz
    float femurCos =
        (FEMUR_LEN * FEMUR_LEN + distance * distance - TIBIA_LEN * TIBIA_LEN) /
        (2.0f * FEMUR_LEN * distance);

    femurCos = constrain(femurCos, -1.0f, 1.0f);

    float femurInnerAngle = acosf(femurCos);
    result.debug.femurInnerAngle = femurInnerAngle;

    // 7. Tibia-Winkel über Kosinussatz
    float tibiaCos =
        (FEMUR_LEN * FEMUR_LEN + TIBIA_LEN * TIBIA_LEN - distance * distance) /
        (2.0f * FEMUR_LEN * TIBIA_LEN);

    tibiaCos = constrain(tibiaCos, -1.0f, 1.0f);

    float tibiaInnerAngle = acosf(tibiaCos);
    result.debug.tibiaInnerAngle = tibiaInnerAngle;

    if (isnan(femurInnerAngle) || isnan(tibiaInnerAngle))
    {
        result.reachable = false;
        return result;
    }

    // 8. Coxa-Winkel aus x/y
    float coxaRad = atan2f(targetPosition.y, targetPosition.x); // Radiant

    // 9. Ergebnis in Grad
    result.angles.coxa = coxaRad * 180.0f / HEXAPOD_PI; // Grad
    result.angles.femur = (baseAngle + femurInnerAngle) * 180.0f / HEXAPOD_PI;
    result.angles.tibia = tibiaInnerAngle * 180.0f / HEXAPOD_PI;

    result.reachable = true;

    return result;
}