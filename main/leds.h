//
// Created by urdmi on 22.11.2023.
//

#ifndef DIPLOM_ESP32_LEDS_H
#define DIPLOM_ESP32_LEDS_H

#define LED_RED 3
#define LED_GREEN 4
#define LED_BLUE 5
#define LED_YELLOW 18
#define LED_WHITE 19

void leds_init();
void leds_flash(int led, int time);

#endif //DIPLOM_ESP32_LEDS_H
