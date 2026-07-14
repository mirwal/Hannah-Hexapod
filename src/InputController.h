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
    float normalizeAxis(const uint8_t raw, const uint8_t rawMin, const uint8_t rawMax) const;
};