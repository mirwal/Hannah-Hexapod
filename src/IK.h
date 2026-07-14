// IK.h
#pragma once
#include <cmath>
#include <cstdint>
#include "Vector3D.h"

class IK
{
public:
    /// Schritt-Offset in Body-Frame (Home + Swing/Stance) für ein Bein
    /// legIndex: 0…5, phase∈[0,1), dirX/dirY∈[–1,1]
    Vector3D computeStepOffset(uint8_t legIndex, float phase,
                               float dirX, float dirY, float liftH,
                               float Lmax);

    /// 2D-IK im lokalen Bein-Frame (nach Coxa-Rotation):
    /// p.x/p.y ist Fußziel im Bein-Frame, p.z Höhe
    void solveLegIK(float x, float y, float z,
                    float &outCoxa, float &outFemur, float &outTibia);

    /// Kombi-Routine (für IDLE/POSING, oder wenn Du beides auf einmal willst)
    void compute(uint8_t legIndex,
                 float &outCoxa, float &outFemur, float &outTibia,
                 float phase, float dirX, float dirY, float liftH,
                 float Lmax);

private:
    // Mounts, HomeOffsets, Längen als constexpr in IK.cpp
};
