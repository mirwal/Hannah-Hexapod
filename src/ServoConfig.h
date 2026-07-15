// ServoConfig.h
#pragma once

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

// !!!!! zum TESTEN !!!!!!!!!!!!! //
inline constexpr float TEST_RANGE_DEG = 20.0f;
inline constexpr float TEST_STEP_LENGTH_MM = 30.0f;