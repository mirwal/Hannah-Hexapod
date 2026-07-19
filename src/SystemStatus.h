// SystemStatus.h
#pragma once
#include <Arduino.h>
#include <cstdint>
#include "WS2812B.h"

inline constexpr uint8_t ERROR_LED_PIN = 13;

enum class SystemFlag : uint16_t
{
    None = 0,
    ServoNotFound = 1 << 0,
    ServoAngleBlocked = 1 << 1,
    IkNotReachable = 1 << 2,
    FunkOffline = 1 << 3,
    BusError = 1 << 4
};
class SystemStatus
{
public:
    explicit SystemStatus(uint8_t ledPin) : ledPin(ledPin) {}

    void begin()
    {
        pinMode(ledPin, OUTPUT);
        digitalWrite(ledPin, LOW);

        led.begin();
        delay(100);

        led.clear();
        led.show();
        delay(100);

        led.setBrightness(50);

        for (int i = 0; i < 60; i += 10)
        {
            led.setPixel(i, 10, 0, 0); // Rot
            Serial.println(i);
        }

        led.show();
    }

    void setFlag(SystemFlag flag)
    {
        flags |= static_cast<uint16_t>(flag);
        led.setPixel(35, Colors::RED); // Rot
        led.show();
    }

    void clear()
    {
        flags = 0;
        led.setPixel(35, Colors::OFF); // Aus
        led.show();
    }
    void clearFlag(SystemFlag flag)
    {
        flags &= ~static_cast<uint16_t>(flag);

        if (flags == 0)
        {
            digitalWrite(ledPin, LOW);
        }
    }

    bool hasError() const
    {
        return flags != 0;
    }

    uint16_t getFlags() const
    {

        return flags;
    }

private:
    uint8_t ledPin;
    uint16_t flags = 0;
    WS2812B led;
};