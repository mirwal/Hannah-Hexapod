// IK.cpp
#include "IK.h"
#include "HexapodGeometry.h"
#include <algorithm>
#include <Arduino.h>

Vector3D IK::computeStepOffset(uint8_t i,
                               float phase,
                               float dirX,
                               float dirY,
                               float liftH,
                               float Lmax)
{
    // Home-Position + Mount
    Vector3D base = mountPos[i] + FOOT_HOME_LOCAL;
    // Serial.printf("Leg %d: base= (%.1f, %.1f, %.1f)\n",                  i, base.x, base.y, base.z);

    // Tripod-Gruppen
    bool isGroupA = (i % 2) == 0;
    bool swing = (phase < 0.5f ? isGroupA : !isGroupA);
    float legP = swing
                     ? (phase < 0.5f ? phase * 2.0f : (phase - 0.5f) * 2.0f)
                     : (1.0f - (phase < 0.5f ? phase * 2.0f : (phase - 0.5f) * 2.0f));

    //  Schrittweite (hier Lmax schon skaliert mit mag vorher!)
    float L = Lmax;

    // Swing vs Stance
    Vector3D stepOffset{0, 0, 0};
    if (swing)
    {
        float s = 0.5f * (1.0f - std::cos(2 * M_PI * legP));
        stepOffset.x = -dirX * L * s;
        stepOffset.y = -dirY * L * s;
        stepOffset.z = liftH * std::sin(M_PI * legP);
    }
    else
    {
        stepOffset.x = -dirX * L * legP;
        stepOffset.y = -dirY * L * legP;
        stepOffset.z = 0.0f;
    }
    // Serial.printf("Leg %d: stepOff=(%.1f, %.1f, %.1f)  dir=(%.2f,%.2f)\n",                  i,                  stepOffset.x, stepOffset.y, stepOffset.z,                  dirX, dirY);

    Vector3D target = base + stepOffset;
    // Serial.printf("Leg %d: target = (%.1f, %.1f, %.1f)\n\n",                  i, target.x, target.y, target.z);
    return target;
}

void IK::solveLegIK(float x, float y, float z,
                    float &outCoxa, float &outFemur, float &outTibia)
{
    // 1) Coxa-Winkel
    outCoxa = std::atan2(y, x) * 180.0f / M_PI;

    // 2) projizieren in XZ
    float dx = x - COXA_LEN;
    float proj = std::sqrt(dx * dx + z * z);

    // 3) Femur
    float a1 = std::atan2(z, dx);
    float a2 = 0.0f;
    if (proj > 1e-3f)
    {
        float c = (FEMUR_LEN * FEMUR_LEN + proj * proj - TIBIA_LEN * TIBIA_LEN) / (2 * FEMUR_LEN * proj);
        a2 = std::acos(std::clamp(c, -1.0f, 1.0f));
    }
    outFemur = (a1 + a2) * 180.0f / M_PI;

    // 4) Tibia
    float a3 = 0.0f;
    if (proj > 1e-3f)
    {
        float c = (FEMUR_LEN * FEMUR_LEN + TIBIA_LEN * TIBIA_LEN - proj * proj) / (2 * FEMUR_LEN * TIBIA_LEN);
        a3 = std::acos(std::clamp(c, -1.0f, 1.0f));
    }
    outTibia = a3 * 180.0f / M_PI;
}

void IK::compute(uint8_t legIndex, // 0…5
                 float &outCoxa,   // Coxa-Winkel in Grad
                 float &outFemur,  // Femur-Winkel in Grad
                 float &outTibia,  // Tibia-Winkel in Grad
                 float phase,      // 0…1, 0=Start, 1=Ende
                 float dirX,       // Richtung X, normiert auf [–1, 1]
                 float dirY,       // Richtung Y, normiert auf [–1, 1]
                 float liftH,      // Höhe des Schritts in mm, 0=keine Hebung
                 float Lmax)       // Schrittweite in mm, normiert auf [0, L_MAX]
{

    float ca = mountPhi[legIndex]; // Mount-Winkel (in rad) für Bein legIndex
                                   // 2) Globalen Joystick-Vektor in lokales Bein-Frame drehen

    // ➊ VORHER: originaler globaler Vektor
    // Serial.printf("Leg %d: global dirX=%.2f dirY=%.2f  ca=%.1f°\n",                  legIndex, dirX, dirY, ca * 180.0f / M_PI);

    // 3) Jetzt mit dem lokal gedrehten Vektor den Fuß-Offset berechnen
    // (in Body-Frame, also mit HomeOffset und MountPos)
    Vector3D target = computeStepOffset(legIndex, phase, dirX, dirY, liftH, Lmax);

    // ❶ Vektor von Coxa-Achse (mountPos) zum Fuß (target) bilden
    // Vector3D rel = {
    //     target.x - mountPos[legIndex].x,
    //     target.y - mountPos[legIndex].y,
    //     target.z - mountPos[legIndex].z};

    // ❷ Diesen REL-Vektor in Dein lokales Bein-Frame rotieren
    // float lx = std::cos(ca) * rel.x + std::sin(ca) * rel.y;
    // float ly = -std::sin(ca) * rel.x + std::cos(ca) * rel.y;
    // float lz = rel.z;

    // ❷ Diesen REL-Vektor in Dein lokales Bein-Frame rotieren
    float lx = std::cos(ca) * target.x + std::sin(ca) * target.y;
    float ly = -std::sin(ca) * target.x + std::cos(ca) * target.y;
    float lz = target.z;

    // Serial.printf("    → lx=%.1f  ly=%.1f\n", lx, ly);
    solveLegIK(lx, ly, lz, outCoxa, outFemur, outTibia);
}
