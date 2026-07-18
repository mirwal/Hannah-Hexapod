#pragma once
#include <cstdint>
constexpr uint8_t LED_COUNT = 60;
constexpr uint8_t LED_PIN = 35;

class WS2812B
{
public:
    WS2812B();
    void begin(uint8_t pin = LED_PIN);
    void setColor(int r, int g, int b);
    void show();
    void clear();
};