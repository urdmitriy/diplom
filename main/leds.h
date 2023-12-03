//
// Created by urdmi on 22.11.2023.
//

#ifndef DIPLOM_ESP32_LEDS_H
#define DIPLOM_ESP32_LEDS_H

typedef enum {
 LED_RED = 3,
 LED_GREEN = 4,
 LED_BLUE = 5,
 LED_YELLOW = 18,
 LED_WHITE = 19,
} leds_e;

void leds_init();
void leds_flash(leds_e led, int time);

#endif //DIPLOM_ESP32_LEDS_H
