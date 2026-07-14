// ServoBus.h
#pragma once
/**
 * Hardware-Schnittstelle zu den Dynamixel-Servos von HANNAH.
 *
 * ServoBus kennt die Dynamixel-Kommunikation, Registeradressen und Umrechnungen,
 * aber nicht die Beinlogik, Gangart oder inverse Kinematik.
 */

#include <Dynamixel2Arduino.h>

// EEPROM Area (nichtflüchtiger Speicher)
#define ADDR_MODEL_NUMBER_L 0
#define ADDR_MODEL_NUMBER_H 1
#define ADDR_VERSION 2
#define ADDR_ID 3
#define ADDR_BAUD_RATE 4
#define ADDR_RETURN_DELAY_TIME 5
#define ADDR_CW_ANGLE_LIMIT_L 6
#define ADDR_CW_ANGLE_LIMIT_H 7
#define ADDR_CCW_ANGLE_LIMIT_L 8
#define ADDR_CCW_ANGLE_LIMIT_H 9
#define ADDR_SYSTEM_DATA2 10
#define ADDR_LIMIT_TEMPERATURE 11
#define ADDR_DOWN_LIMIT_VOLTAGE 12
#define ADDR_UP_LIMIT_VOLTAGE 13
#define ADDR_MADDR_TORQUE_L 14
#define ADDR_MADDR_TORQUE_H 15
#define ADDR_RETURN_LEVEL 16
#define ADDR_ALARM_LED 17
#define ADDR_ALARM_SHUTDOWN 18
#define ADDR_OPERATING_MODE 19
#define ADDR_DOWN_CALIBRATION_L 20
#define ADDR_DOWN_CALIBRATION_H 21
#define ADDR_UP_CALIBRATION_L 22
#define ADDR_UP_CALIBRATION_H 23

// RAM Area (flüchtiger Speicher)
#define ADDR_TORQUE_ENABLE 24
#define ADDR_LED 25
#define ADDR_CW_COMPLIANCE_MARGIN 26
#define ADDR_CCW_COMPLIANCE_MARGIN 27
#define ADDR_CW_COMPLIANCE_SLOPE 28
#define ADDR_CCW_COMPLIANCE_SLOPE 29
#define ADDR_GOAL_POSITION_L 30
#define ADDR_GOAL_POSITION_H 31
#define ADDR_MOVING_SPEED_L 32
#define ADDR_MOVING_SPEED_H 33
#define ADDR_TORQUE_LIMIT_L 34
#define ADDR_TORQUE_LIMIT_H 35
#define ADDR_PRESENT_POSITION_L 36
#define ADDR_PRESENT_POSITION_H 37
#define ADDR_PRESENT_SPEED_L 38
#define ADDR_PRESENT_SPEED_H 39
#define ADDR_PRESENT_LOAD_L 40
#define ADDR_PRESENT_LOAD_H 41
#define ADDR_PRESENT_VOLTAGE 42
#define ADDR_PRESENT_TEMPERATURE 43
#define ADDR_REGISTERED_INSTRUCTION 44
#define ADDR_MOVING 46
#define ADDR_LOCK 47
#define ADDR_PUNCH_L 48
#define ADDR_PUNCH_H 49

#define TX_DELAY_TIME 400   // Zeit in Mikrosekunden, die zwischen den Paketen gewartet wird
#define ID_CHANGE_ENABLED 0 // Setze auf 1, um die ID-Änderung zu aktivieren, auf 0, um sie zu deaktivieren

class ServoBus
{
public:
    // initalisierung der Hardware-Schnittstelle
    void begin();

    // Servo prüfen, ob er antwortet
    bool ping(uint8_t id);
    uint16_t getModelNumber(uint8_t id);

    // Servo konfigurieren
    bool changeID(uint8_t oldID, uint8_t newID);

    // Servo bewegen
    void setTorqueEnabled(uint8_t id, bool enable);
    void setPosition(uint8_t id, float angleDeg);
    bool setSpeed(uint8_t id, uint16_t speed);

    // Servo abfragen
    float getPresentPosition(uint8_t id, uint8_t unit = 1); // 0 = RAW, 1 = DEGREE

private:
    uint16_t model_number;
    unsigned char Checksum;
    // DEG_TO_DXL ist der Umrechnungsfaktor von Grad zu DXL-Winkel 3.41f
    static constexpr const float DEG_TO_DXL = 1023.0f / 300.0f;
    Dynamixel2Arduino dxl = Dynamixel2Arduino(Serial1, 2); // TX/RX mit DIR-Pin also  Serial1 + DIR-Pin 2
};
