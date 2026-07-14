// ServoBus.cpp
#include "ServoBus.h"
// Control Table definitions for AX-12A (Firmware 1.0)
#define sendData(args) (Serial1.write(args)) // Write Over Serial

void ServoBus::begin()
{
    dxl.begin(1000000); // Initialisiert Serial & Direction-Pin intern!
    dxl.setPortProtocolVersion(1.0);
    Serial.println("ServoBus bereit.");
}

uint16_t ServoBus::getModelNumber(uint8_t id)
{
    return dxl.getModelNumber(id);
}

void ServoBus::setPosition(uint8_t id, float angleDeg)
{
    if (id == 0 || id == 254)
    {
        Serial.println("BLOCK: ungueltige/broadcast Servo-ID");
        return;
    }
    if (id > 18)
    {
        Serial.println("BLOCK: Servo-ID ist größer 18");
        return;
    }
    if (angleDeg < 20.0f || angleDeg > 280.0f)
    {
        Serial.print("BLOCK: Winkel ausserhalb AX-12 Bereich ID ");
        Serial.println(id);
        return;
    }
    uint16_t posValue = (uint16_t)(angleDeg * DEG_TO_DXL); // Umrechnung von Grad in DXL-Winkel
    posValue = constrain(posValue, 0, 1023);               // stellt sicher, dass dein Wert im gültigen Dynamixel-Bereich bleibt
    // dxl.setGoalPosition(id, posValue); // einfache Bewegung, vorher gesetzte Speed bleibt erhalten ist aber zu █ langsam █
    dxl.write(id, ADDR_GOAL_POSITION_L, (uint8_t *)&posValue, 2, 1); // Set Goal Position
}

bool ServoBus::ping(uint8_t id)
{
    bool pingSuccess = false;
    unsigned long startTime = millis();
    do
    {
        pingSuccess = dxl.ping(id);
        delay(20); // Kurze Pause, um die Kommunikation zu stabilisieren
    } while (!pingSuccess && (millis() - startTime < 200)); // 200ms Timeout
    return pingSuccess;
}

float ServoBus::getPresentPosition(uint8_t id, uint8_t unit)
{
    float pos = 0.0f;
    pos = dxl.getPresentPosition(id, 0); // 0 = RAW, 1 = DEGREE

    return pos * (unit == 1 ? 300.0f / 1023.0f : 1.0f); // Umrechnung in Grad oder RAW
}
bool ServoBus::changeID(uint8_t oldID, uint8_t newID)
{
    if (ID_CHANGE_ENABLED == 0)
    {
        Serial.println("ID-Change ist deaktiviert. Bitte ID_CHANGE_ENABLED auf 1 setzen.");
        return false;
    }
    dxl.ledOn(oldID);
    delay(1000);
    return dxl.setID(oldID, newID);
}
void ServoBus::setTorqueEnabled(uint8_t id, bool enable)
{
    if (enable)
    {
        dxl.torqueOn(id);
    }
    else
    {
        dxl.torqueOff(id);
    }

    // uint8_t packet[8];
    // packet[0] = 0xFF;
    // packet[1] = 0xFF;
    // packet[2] = id;
    // packet[3] = 0x04; // Paketlänge ab Instruction
    // packet[4] = 0x03; // WRITE Instruction
    // packet[5] = 24;   // Adresse: Torque Enable
    // packet[6] = enable ? 1 : 0;
    // packet[7] = ~(packet[2] + packet[3] + packet[4] + packet[5] + packet[6]); // Checksum

    // Serial1.write(packet, 8);
    // Serial1.flush();
    delayMicroseconds(50);
}

bool ServoBus::setSpeed(uint8_t id, uint16_t speed)
{
    if (id == 0 || id == 254)
    {
        Serial.println("BLOCK: ungueltige/broadcast Servo-ID");
        return false;
    }

    if (id > 18)
    {
        Serial.println("BLOCK: Servo-ID ist größer 18");
        return false;
    }

    if (speed > 1023)
    {
        Serial.print("BLOCK: Geschwindigkeit ausserhalb AX-12 Bereich ID ");
        Serial.println(id);
        return false;
    }
    return dxl.write(id, ADDR_MOVING_SPEED_L, (uint8_t *)&speed, 2, 1);
}