// GaitConfig.h
#pragma once

// Gait parameters
inline constexpr float L_MAX = 20.0f;     // max step length [mm]
inline constexpr float T_MIN = 0.2f;      // fastest cycle [s]
inline constexpr float T_MAX = 1.0f;      // slowest cycle [s]
inline constexpr float LIFT_H = 10.0f;    // lift height [mm]
inline constexpr float DEADZONE = 0.05f;  // min move magnitude
inline constexpr float CYCLE_TIME = 0.5f; // full gait cycle [s]