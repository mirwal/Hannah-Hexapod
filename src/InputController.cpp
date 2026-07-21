// InputController.cpp
#include "InputController.h"
#include "HexapodTypes.h"
#include "Arduino.h"
#include "Tasten.h"

InputController::InputController(Tasten &tast) : funkSteuerung(tast) {}

void InputController::update()
{
    const uint32_t now = millis();

    // Einmal-Befehl immer sofort zurücksetzen
    control.resetStatusRequested = false;
    if (now - lastReadTime < 50)
    {
        return; // nur alle 50 ms lesen
    }

    lastReadTime = now;

    funkSteuerung.funkDatenEinlesen();

    interpretPacket(funkSteuerung.getFunkPacket());
    // daraus dirX, dirY, yaw setzen
}

void InputController::interpretPacket(const FunkPacket &packet)
{
    control.online = packet.online;
    if (!packet.online)
    {
        control.moveDirBody = {0.0f, 0.0f, 0.0f};
        control.pitch = 0.0f;
        control.yaw = 0.0f;

        control.moveMode = HexapodMoveMode::Idle;
        control.function = HexapodFunction::Stop;
        control.online = false;
        control.debugMode = HexapodDebugMode::Off;
        control.resetStatusRequested = false;
        control.trainer = false;

        lastTrainer = false;
        return;
    }

    control.moveDirBody.x = normalizeAxis(packet.hl_lr, 794, 222, 0.02f);
    control.moveDirBody.y = normalizeAxis(packet.hl_ud, 277, 744, -0.06f);
    control.yaw = normalizeAxis(packet.hr_lr, 234, 805, 0.0f);
    control.pitch = normalizeAxis(packet.hr_ud, 796, 209, -0.05f);
    control.poti = normalizeRange(packet.poti, 0, 1023);
    control.fader = normalizeRange(packet.fader, 1023, 0);
    control.debugMode = interpretDebugMode(packet.flap);
    control.moveMode = interpretMoveMode(packet);
    control.trainer = packet.trainer;
    control.clearTaster = packet.clearTaster;
    control.backTaster = packet.backTaster;
    control.encoderTaster = !packet.encoderTaster;
    control.sonderTaste5 = packet.sonderTaste5;
    control.sonderTaste6 = packet.sonderTaste6;
    control.sonderTaste7 = packet.sonderTaste7;
    control.sonderTaste8 = packet.sonderTaste8;
    control.taster1 = packet.taster3;
    control.taster2 = packet.taster4;
    control.taster3 = packet.taster5;
    control.taster4 = packet.taster6;
    control.taster5 = packet.taster7;
    control.taster6 = packet.taster8;
    control.taster7 = packet.taster1;
    control.taster8 = packet.taster2;

    control.moveMode = interpretMoveMode(packet);

    // Flankenerkennung:
    // true nur in dem Moment, wo der Taster neu gedrückt wurde
    control.resetStatusRequested = packet.trainer && !lastTrainer;
    // Hier wird der aktuelle Zustand für den nächsten Durchlauf gemerkt
    lastTrainer = control.trainer;
}

HexapodMoveMode InputController::interpretMoveMode(const FunkPacket &packet) const
{
    uint8_t modeMask = 0;
    // TODO: Hier könnte man die Logik anpassen, um verschiedene Kombinationen von Tasten zu interpretieren.
    if (packet.hl1)
        modeMask |= 0b001;
    if (packet.hl2)
        modeMask |= 0b010;
    if (packet.hl3)
        modeMask |= 0b011;
    if (packet.hr1)
        modeMask |= 0b100;
    if (packet.hr2)
        modeMask |= 0b101;
    if (packet.hr3)
        modeMask |= 0b110;

    switch (modeMask)
    {
    case 0b001:
        return HexapodMoveMode::Walking;

    case 0b010:
        return HexapodMoveMode::Posing;

    case 0b011:
        return HexapodMoveMode::Testing;

    default:
        return HexapodMoveMode::Idle;
    }
}
float InputController::normalizeAxis(uint16_t raw, uint16_t rawMin, uint16_t rawMax, float offset) const
{
    // Verschiebe und skaliere: Mitte → 0.0, Rand → ±1.0
    float mid = ((rawMin + rawMax) * 0.5f);
    float halfR = ((rawMax - rawMin) * 0.5f);
    float norm = ((raw - mid) / halfR) + offset;
    // Deadzone anpassen
    if (norm >= -0.02f && norm <= 0.02f)
        norm = 0.0f;

    // Sicherstellen, dass wir nicht out-of-bounds rauslaufen:
    if (norm > 1.0f)
        norm = 1.0f;
    else if (norm < -1.0f)
        norm = -1.0f;

    return norm;
}
float InputController::normalizeRange(uint16_t raw, uint16_t rawMin, uint16_t rawMax) const
{
    if (rawMax == rawMin)
        return 0.0f;

    float norm = (float)(raw - rawMin) / (float)(rawMax - rawMin);

    if (norm < 0.02f)
        norm = 0.0f;
    else if (norm > 0.98f)
        norm = 1.0f;

    if (norm < 0.0f)
        norm = 0.0f;
    else if (norm > 1.0f)
        norm = 1.0f;

    return norm;
}
HexapodDebugMode InputController::interpretDebugMode(uint8_t flap) const
{

    if (flap > 1 && flap < 100)
        return HexapodDebugMode::Minimal;
    else if (flap > 100)
        return HexapodDebugMode::Maximal;
    else
        return HexapodDebugMode::Off;
}