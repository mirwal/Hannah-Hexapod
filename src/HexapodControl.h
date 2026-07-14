// HexapodControl.h
#pragma once
/**
 * InputController ──◆ HexapodControl
 * HexapodControl ist der aufbereitete Steuerdatensatz zwischen
 * InputController und Hexapod. Der InputController füllt diese Struktur
 * aus den rohen Funkdaten, der Hexapod liest sie zur Bewegungssteuerung.
 */
#include <cstdint>

enum class HexapodFunction : uint8_t
{
    None,
    Walk,
    Pose,
    Test,
    Stop
};

enum class HexapodMoveMode : uint8_t // Wunsch / Steuerabsicht von außen
{
    Idle,
    Walking,
    Posing,
    Testing
};

enum class HexapodDebugMode : uint8_t
{
    Off = 1, // debug aus, keine Ausgabe
    Minimal, // minimale Debug-Ausgabe
    Maximal  // maximale Debug-Ausgabe
};
struct HexapodControl
{
    bool online = false;

    float dirX = 0.0f;
    float dirY = 0.0f;
    float rot = 0.0f;
    float pitch = 0.0f;
    bool trainer = false;
    bool resetStatusRequested = false;

    HexapodDebugMode debugMode = HexapodDebugMode::Off;
    HexapodFunction function = HexapodFunction::None;
    HexapodMoveMode moveMode = HexapodMoveMode::Idle;
    // GaitType gait = GaitType::Tripod;
};
