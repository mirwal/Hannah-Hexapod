// HexapodControl.h
#pragma once
/**
 * InputController ──◆ HexapodControl
 * HexapodControl ist der aufbereitete Steuerdatensatz zwischen
 * InputController und Hexapod. Der InputController füllt diese Struktur
 * aus den rohen Funkdaten, der Hexapod liest sie zur Bewegungssteuerung.
 */
#include <cstdint>
#include "HexapodTypes.h"

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

    MoveVectorBody moveDirBody = {0.0f, 0.0f, 0.0f}; // x/y Bewegungsrichtung im Body-System
    float yaw = 0.0f;                                // Drehwunsch um die Z-Achse
    float pitch = 0.0f;
    float poti = 0.0f;  // Poti-Wert, normiert auf [0.0, 1.0]
    float fader = 0.0f; // Fader-Wert, normiert auf [0.0, 1.0]

    // Taster
    bool taster1 = false;
    bool taster2 = false;
    bool taster3 = false;
    bool taster4 = false;
    bool taster5 = false;
    bool taster6 = false;
    bool taster7 = false;
    bool taster8 = false;

    // Taster / Sonderfunktionen
    bool trainer = false;
    bool clearTaster = false;
    bool backTaster = false;
    bool encoderTaster = false;
    bool sonderTaste5 = false;
    bool sonderTaste6 = false;
    bool sonderTaste7 = false;
    bool sonderTaste8 = false;

    bool resetStatusRequested = false;

    HexapodDebugMode debugMode = HexapodDebugMode::Off;
    HexapodFunction function = HexapodFunction::None;
    HexapodMoveMode moveMode = HexapodMoveMode::Idle;
    // GaitType gait = GaitType::Tripod;
};
