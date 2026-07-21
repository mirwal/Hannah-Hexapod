// InputController.h
#pragma once
/**
 * InputController ──◆ HexapodControl
 * InputController wertet die rohen Funkdaten aus der Tasten-Klasse aus
 * und übersetzt sie in eine aufbereitete HexapodControl-Struktur.
 *
 * Er trennt damit die Funksteuerung von der eigentlichen Hexapod-Logik,
 * sodass HANNAH nur noch fertige Bewegungs-, Modus- und Steuerinformationen bekommt.
 */
#include <cstdint>
#include "Tasten.h"
#include "HexapodControl.h"
class InputController
{
public:
    InputController(Tasten &tast);
    void update();
    const HexapodControl &getControl() const { return control; }
    HexapodDebugMode interpretDebugMode(uint8_t flap) const;

private:
    Tasten &funkSteuerung;
    HexapodControl control;

    uint32_t lastReadTime = 0;
    bool lastTrainer = false;
    void interpretPacket(const FunkPacket &packet);
    HexapodMoveMode interpretMoveMode(const FunkPacket &packet) const;
    float normalizeAxis(const uint16_t raw, const uint16_t rawMin, const uint16_t rawMax, const float offset) const;
    float normalizeRange(uint16_t raw, uint16_t rawMin, uint16_t rawMax) const;
};