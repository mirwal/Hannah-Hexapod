#include "WS2812B.h"

uint8_t WS2812B::drawingMemory[LED_COUNT * 3] = {};

DMAMEM uint8_t WS2812B::displayMemory[LED_COUNT * 12] = {};

WS2812B::WS2812B()
    : ws2812b_Leds(
          LED_COUNT,
          displayMemory,
          drawingMemory,
          LED_PIN,
          WS2812_GRB)
// WS2812_RGB)
{
}
// WS2812Serial led_Ring;