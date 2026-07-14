// HexapodGeometry.h
#pragma once
#include <cstdint>
#include "Vector3D.h"

// Servo-Winkelbereiche [Grad]
inline constexpr float SERVO_MIN_DEG = 60.0f;
inline constexpr float SERVO_MAX_DEG = 220.0f;

inline constexpr float COXA_SERVO_CENTER_DEG = 150.0f;
inline constexpr float FEMUR_SERVO_CENTER_DEG = 150.0f;
inline constexpr float TIBIA_SERVO_CENTER_DEG = 150.0f;

inline constexpr float COXA_SERVO_OFFSET_DEG = 0.0f;
inline constexpr float FEMUR_SERVO_OFFSET_DEG = 0.0f;
inline constexpr float TIBIA_SERVO_OFFSET_DEG = 78.0f;

inline constexpr int8_t COXA_SERVO_DIR = 1;
inline constexpr int8_t FEMUR_SERVO_DIR = 1;
inline constexpr int8_t TIBIA_SERVO_DIR = -1;

inline constexpr float TEST_RANGE_DEG = 20.0f;

// Anzahl der Beine
inline constexpr uint8_t LEG_COUNT = 6;

// Mathematische Konstanten
inline constexpr float HEXAPOD_PI = 3.14159265358979323846f;
inline constexpr float HEXAPOD_HALF = 0.5f;
inline constexpr float HEXAPOD_SQRT3_2 = 0.8660254037844386f;

// Coxa-Montagepunkte im Körper-Koordinatensystem
inline constexpr float R = 115.0f;

// Beinlängen [mm]
inline constexpr float COXA_LEN = 50.0f;
inline constexpr float FEMUR_LEN = 64.2f;
inline constexpr float TIBIA_LEN = 94.8f;

// Abstand Fußspitze ↔ Coxa-Achse [mm]
inline constexpr float AF_CA = 50.0f;

// Lokale Home-Position der Fußspitze relativ zur Coxa-Achse [mm]
// x: Abstand nach außen vom Körper
// y: seitlicher Versatz innerhalb der Beinebene
// z: Höhe, negativ nach unten
inline constexpr Vector3D FOOT_HOME_LOCAL = {AF_CA, 0.0f, -80.0f};

// Coxa-Montagepunkte im Körper-Koordinatensystem
inline constexpr Vector3D mountPos[LEG_COUNT] = {
    {R * HEXAPOD_HALF, R *HEXAPOD_SQRT3_2, 0.0f},   //  60°
    {-R * HEXAPOD_HALF, R *HEXAPOD_SQRT3_2, 0.0f},  // 120°
    {-R, 0.0f, 0.0f},                               // 180°
    {-R * HEXAPOD_HALF, -R *HEXAPOD_SQRT3_2, 0.0f}, // 240°
    {R * HEXAPOD_HALF, -R *HEXAPOD_SQRT3_2, 0.0f},  // 300°
    {R, 0.0f, 0.0f}                                 //   0°
};

// Montagewinkel der sechs Beine [rad]
inline constexpr float mountPhi[LEG_COUNT] = {
    1.0f * HEXAPOD_PI / 3.0f, //  60°
    2.0f * HEXAPOD_PI / 3.0f, // 120°
    3.0f * HEXAPOD_PI / 3.0f, // 180°
    4.0f * HEXAPOD_PI / 3.0f, // 240°
    5.0f * HEXAPOD_PI / 3.0f, // 300°
    0.0f                      //   0°
};

// 80 mm Abstand bei Leg0 im Winkel 60°:
//   x = 80*cos(60°) =  40.0
//   y = 80*sin(60°) =  69.28
// gerundet auf 2 Nachkommastellen

// static constexpr Vector3D footHomeOffsetBody[LEG_COUNT] = {
//     {AF_CA, 0.0f, -30.0f},                    //  0°
//     {AF_CA * HALF, AF_CA *SQRT3_2, -30.0f},   //  60°
//     {-AF_CA * HALF, AF_CA *SQRT3_2, -30.0f},  // 120°
//     {-AF_CA, 0.0f, -30.0f},                   // 180°
//     {-AF_CA * HALF, -AF_CA *SQRT3_2, -30.0f}, // 240°
//     {AF_CA * HALF, -AF_CA *SQRT3_2, -30.0f}   // 300°
// };

// Home-Positionen der Füße im Körper-Koordinatensystem [mm]
// static constexpr Vector3D homePos[LEG_COUNT] = {
//     {40.00f, 69.28f, -30.0f},   // Bein 0 (60°)
//     {80.00f, 0.00f, -30.0f},    // Bein 1 ( 0°)
//     {40.00f, -69.28f, -30.0f},  // Bein 2 (–60°)
//     {-40.00f, -69.28f, -30.0f}, // Bein 3 (–120°)
//     {-80.00f, 0.00f, -30.0f},   // Bein 4 (180°)
//     {-40.00f, 69.28f, -30.0f}   // Bein 5 (120°)

// };
// inline constexpr Vector3D footHomeOffsetBody[LEG_COUNT] = {
//     {40.0f, 69.28f, -30.0f},   //  60°
//     {-40.0f, 69.28f, -30.0f},  // 120°
//     {-80.0f, 0.0f, -30.0f},    // 180°
//     {-40.0f, -69.28f, -30.0f}, // 240°
//     {40.0f, -69.28f, -30.0f},  // 300°
//     {80.0f, 0.0f, -30.0f}      //   0°
// };
