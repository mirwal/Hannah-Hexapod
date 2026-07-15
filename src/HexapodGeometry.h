// HexapodGeometry.h
#pragma once
#include <cstdint>
#include "Vector3D.h"

/*
Namens-Suffixe:
Diese Endungen zeigen Bedeutung, Einheit oder Koordinatensystem.

_MM     → Millimeter / Länge
_DEG    → Grad
_RAD    → Radiant
_COUNT  → Anzahl
_PIN    → Pin-Nummer
_ID     → Servo-ID / Geräte-ID

_POS    → Position / Punkt
_VEC    → Vektor / Richtung / Abstand
_ANGLE  → Winkel
_LOCAL  → lokales Bein-Koordinatensystem
_MOUNT  → fester Montagepunkt / mechanischer Befestigungspunkt
_BODY   → Körper-Koordinatensystem
_WORLD  → Welt-/Raum-Koordinatensystem

*/

// Anzahl der Beine
inline constexpr uint8_t LEG_COUNT = 6;

// Mathematische Konstanten
inline constexpr float HEXAPOD_PI = 3.14159265358979323846f;
inline constexpr float HEXAPOD_HALF = 0.5f;
inline constexpr float HEXAPOD_SQRT3_2 = 0.8660254037844386f;

// Abstand Körpermittelpunkt → Coxa-Achse [mm]
inline constexpr float BODY_MOUNT_RADIUS_MM = 115.0f;

// Beinlängen [mm]
inline constexpr float COXA_LEN_MM = 50.0f;
inline constexpr float FEMUR_LEN_MM = 64.2f;
inline constexpr float TIBIA_LEN_MM = 94.8f;

// Vom Coxa-Montagepunkt aus 50 mm nach vorne im lokalen Bein-Koordinatensystem P_local = (50, 0)
inline constexpr float FOOT_HOME_X_FROM_COXA_LOCAL_MM = 50.0f;
inline constexpr float FOOT_HOME_Y_FROM_COXA_LOCAL_MM = 0.0f;
inline constexpr float FOOT_HOME_Z_FROM_COXA_LOCAL_MM = -80.0f;

// Lokale Home-Position der Fußspitze relativ zur Coxa-Achse [mm]
// x: Abstand nach außen vom Körper
// y: seitlicher Versatz innerhalb der Beinebene
// z: Höhe, negativ nach unten
inline constexpr Vector3D FOOT_HOME_POS_LOCAL_MM =
    {
        FOOT_HOME_X_FROM_COXA_LOCAL_MM,
        FOOT_HOME_Y_FROM_COXA_LOCAL_MM,
        FOOT_HOME_Z_FROM_COXA_LOCAL_MM};

// Coxa-Montagepunkte im Körper-Koordinatensystem
inline constexpr Vector3D COXA_MOUNT_POS_BODY_MM[LEG_COUNT] = {
    {BODY_MOUNT_RADIUS_MM * HEXAPOD_HALF, BODY_MOUNT_RADIUS_MM *HEXAPOD_SQRT3_2, 0.0f},   //  60°
    {-BODY_MOUNT_RADIUS_MM * HEXAPOD_HALF, BODY_MOUNT_RADIUS_MM *HEXAPOD_SQRT3_2, 0.0f},  // 120°
    {-BODY_MOUNT_RADIUS_MM, 0.0f, 0.0f},                                                  // 180°
    {-BODY_MOUNT_RADIUS_MM * HEXAPOD_HALF, -BODY_MOUNT_RADIUS_MM *HEXAPOD_SQRT3_2, 0.0f}, // 240°
    {BODY_MOUNT_RADIUS_MM * HEXAPOD_HALF, -BODY_MOUNT_RADIUS_MM *HEXAPOD_SQRT3_2, 0.0f},  // 300°
    {BODY_MOUNT_RADIUS_MM, 0.0f, 0.0f}                                                    //   0°
};

// Montagewinkel der sechs Beine [rad]
inline constexpr float COXA_MOUNT_ANGLE_BODY_RAD[LEG_COUNT] = {
    1.0f * HEXAPOD_PI / 3.0f, //  60°
    2.0f * HEXAPOD_PI / 3.0f, // 120°
    3.0f * HEXAPOD_PI / 3.0f, // 180°
    4.0f * HEXAPOD_PI / 3.0f, // 240°
    5.0f * HEXAPOD_PI / 3.0f, // 300°
    0.0f                      //   0°
};
