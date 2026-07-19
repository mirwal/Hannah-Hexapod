#pragma once

#include <Arduino.h>
#include <WS2812Serial.h>
struct Color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

namespace Colors
{
    /// Primär-, Sekundär- und Grundfarben
    inline constexpr Color OFF{0, 0, 0};
    inline constexpr Color RED{255, 0, 0};
    inline constexpr Color GREEN{0, 255, 0};
    inline constexpr Color BLUE{0, 0, 255};
    inline constexpr Color YELLOW{255, 255, 0};
    inline constexpr Color CYAN{0, 255, 255};
    inline constexpr Color MAGENTA{255, 0, 255};
    inline constexpr Color WHITE{255, 255, 255};
    inline constexpr Color BLACK{0, 0, 0};
    inline constexpr Color GRAY{128, 128, 128};
    inline constexpr Color LIGHT_GRAY{211, 211, 211};
    inline constexpr Color DARK_GRAY{169, 169, 169};

    ///  Warme Töne Rot orange braun
    inline constexpr Color ORANGE{255, 165, 0};
    inline constexpr Color DARK_ORANGE{255, 140, 0};
    inline constexpr Color CORAL{255, 127, 80};
    inline constexpr Color TOMATO{255, 99, 71};
    inline constexpr Color CRIMSON{220, 20, 60};
    inline constexpr Color MAROON{128, 0, 0};
    inline constexpr Color BROWN{139, 69, 19};
    inline constexpr Color CHOCOLATE{210, 105, 30};
    /// Gelb- und Goldtöne
    inline constexpr Color GOLD{255, 215, 0};
    inline constexpr Color ORANGE_RED{255, 69, 0};
    inline constexpr Color BEIGE{245, 245, 220};
    /// Grüntöne
    inline constexpr Color LIME{0, 255, 0};
    inline constexpr Color FOREST_GREEN{34, 139, 34};
    inline constexpr Color OLIVE{128, 128, 0};
    inline constexpr Color DARK_GREEN{0, 100, 0};
    inline constexpr Color MINT{152, 251, 152};
    /// Blautöne
    inline constexpr Color NAVY{0, 0, 128};
    inline constexpr Color SKY_BLUE{135, 206, 235};
    inline constexpr Color DEEP_SKY_BLUE{0, 191, 255};
    inline constexpr Color TURQUOISE{64, 224, 208};
    inline constexpr Color TEAL{0, 128, 128};
    inline constexpr Color STEEL_BLUE{70, 130, 180};
    /// Lila- und Violetttöne
    inline constexpr Color PURPLE{128, 0, 128};
    inline constexpr Color VIOLET{238, 130, 238};
    inline constexpr Color INDIGO{75, 0, 130};
    inline constexpr Color PINK{255, 192, 203};
    inline constexpr Color DEEP_PINK{255, 20, 147};
    inline constexpr Color LAVENDER{230, 230, 250};
    ///
}
class WS2812B
{
public:
    WS2812B();

    void begin() { ws2812b_Leds.begin(); }
    void setPixel(int index, int r, int g, int b) { ws2812b_Leds.setPixel(index, r, g, b); }
    void setPixel(int index, Color color) { ws2812b_Leds.setPixel(index, color.red, color.green, color.blue); }
    void setColor(int index, int r, int g, int b) { ws2812b_Leds.setPixelColor(index, r, g, b); }
    void setColor(int index, Color color) { ws2812b_Leds.setPixelColor(index, color.red, color.green, color.blue); }

    void show() { ws2812b_Leds.show(); }
    void clear() { ws2812b_Leds.clear(); }
    void setBrightness(uint8_t brightness) { ws2812b_Leds.setBrightness(brightness); }

private:
    static constexpr uint8_t LED_COUNT = 60;
    static constexpr uint8_t LED_PIN = 35;

    static uint8_t drawingMemory[LED_COUNT * 3];
    static uint8_t displayMemory[LED_COUNT * 12];

    WS2812Serial ws2812b_Leds;
};