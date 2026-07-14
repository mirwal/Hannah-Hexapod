// SystemStatus.h
#pragma once
#include <Arduino.h>
#include <cstdint>

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
    }

    void setFlag(SystemFlag flag)
    {
        flags |= static_cast<uint16_t>(flag);
        digitalWrite(ledPin, HIGH);
    }

    void clear()
    {
        flags = 0;
        digitalWrite(ledPin, LOW);
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
};