// InputController.cpp
#include "InputController.h"
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

    funkSteuerung.lesen();
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

    control.moveDirBody.x = normalizeAxis(packet.hl_lr, 30, 102);
    control.moveDirBody.y = normalizeAxis(packet.hl_ud, 34, 92);
    control.moveDirBody.z = normalizeAxis(packet.hr_lr, 29, 100);
    control.pitch = normalizeAxis(packet.hr_ud, 30, 103);

    control.debugMode = interpretDebugMode(packet.flap);
    control.moveMode = interpretMoveMode(packet);

    // Flankenerkennung:
    // true nur in dem Moment, wo der Taster neu gedrückt wurde
    control.resetStatusRequested = packet.trainer && !lastTrainer;
    control.trainer = packet.trainer;
    // Hier wird der aktuelle Zustand für den nächsten Durchlauf gemerkt
    lastTrainer = packet.trainer;
}
HexapodMoveMode InputController::interpretMoveMode(const FunkPacket &packet) const
{
    uint8_t modeMask = 0;

    if (packet.hl1)
        modeMask |= 0b01;
    if (packet.hl2)
        modeMask |= 0b10;

    switch (modeMask)
    {
    case 0b01:
        return HexapodMoveMode::Walking;

    case 0b10:
        return HexapodMoveMode::Posing;

    case 0b11:
        return HexapodMoveMode::Testing;

    default:
        return HexapodMoveMode::Idle;
    }
}
float InputController::normalizeAxis(uint8_t raw, uint8_t rawMin, uint8_t rawMax) const
{
    // Verschiebe und skaliere: Mitte → 0.0, Rand → ±1.0
    float mid = (rawMin + rawMax) * 0.5f;
    float halfR = (rawMax - rawMin) * 0.5f;
    float norm = (raw - mid) / halfR;

    // Sicherstellen, dass wir nicht out-of-bounds rauslaufen:
    if (norm > 1.0f)
        norm = 1.0f;
    else if (norm < -1.0f)
        norm = -1.0f;

    return norm;
}
HexapodDebugMode InputController::interpretDebugMode(uint8_t flap) const
{
    switch (flap)
    {
    case 1:
        return HexapodDebugMode::Off;

    case 2:
        return HexapodDebugMode::Minimal;

    case 3:
        return HexapodDebugMode::Maximal;

    default:
        return HexapodDebugMode::Off;
    }
}