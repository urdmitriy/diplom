//
// Created by urdmi on 22.11.2023.
//

#include "leds.h"
#include "../../../Users/urdmi/esp/esp-idf/components/driver/gpio/include/driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void leds_init(){
    gpio_reset_pin(LED_RED);
    gpio_reset_pin(LED_GREEN);
    gpio_reset_pin(LED_BLUE);
    gpio_reset_pin(LED_WHITE);
    gpio_reset_pin(LED_YELLOW);

    gpio_set_direction(LED_RED, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_GREEN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_BLUE, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_YELLOW, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_WHITE, GPIO_MODE_OUTPUT);
}

void leds_flash(int led, int time){
    gpio_set_level(led, 1);
    vTaskDelay(pdMS_TO_TICKS(time));
    gpio_set_level(led, 0);
    vTaskDelay(pdMS_TO_TICKS(time));
}