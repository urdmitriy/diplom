//
// Created by urdmi on 22.11.2023.
//

#include "leds.h"
#include "../../../Users/urdmi/esp/esp-idf/components/driver/gpio/include/driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_log.h"

TimerHandle_t timer_led_yellow, timer_led_white, timer_led_red,timer_led_green,timer_led_blue;

void TimerCallbackFunction(TimerHandle_t xTimer ){
    if (xTimer == timer_led_yellow) {
        gpio_set_level(LED_YELLOW, 0);
        ESP_LOGI("LEDS", "LED YELLOW OFF");
    } else if (xTimer == timer_led_white) {
        gpio_set_level(LED_WHITE, 0);
        ESP_LOGI("LEDS", "LED WHITE OFF");
    } else if (xTimer == timer_led_red) {
        gpio_set_level(LED_RED, 0);
        ESP_LOGI("LEDS", "LED RED OFF");
    } else if (xTimer == timer_led_green) {
        gpio_set_level(LED_GREEN, 0);
        ESP_LOGI("LEDS", "LED GREEN OFF");
    } else if (xTimer == timer_led_blue) {
        gpio_set_level(LED_BLUE, 0);
        ESP_LOGI("LEDS", "LED BLUE OFF");
    }
}

void leds_init(){
    ESP_LOGI("LEDS", "LED init begin");
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

    timer_led_yellow = xTimerCreate("timer led", pdMS_TO_TICKS(100), pdFALSE, "timer yellow", TimerCallbackFunction);

    ESP_LOGI("LEDS", "LED init end");
}

void leds_flash(leds_e led, int time){

    gpio_set_level(led, 1);

    switch (led) {
        case LED_YELLOW:
            xTimerChangePeriod(timer_led_yellow, pdMS_TO_TICKS(time),0);
            xTimerStart(timer_led_yellow, 0);
            break;
        case LED_WHITE:
            xTimerChangePeriod(timer_led_white, pdMS_TO_TICKS(time),0);
            xTimerStart(timer_led_white, 0);
            break;
        case LED_GREEN:
            xTimerChangePeriod(timer_led_green, pdMS_TO_TICKS(time),0);
            xTimerStart(timer_led_green, 0);
            break;
        case LED_RED:
            xTimerChangePeriod(timer_led_red, pdMS_TO_TICKS(time),0);
            xTimerStart(timer_led_red, 0);
            break;
        case LED_BLUE:
            xTimerChangePeriod(timer_led_blue, pdMS_TO_TICKS(time),0);
            xTimerStart(timer_led_blue, 0);
            break;
        default:
            break;
    }


    ESP_LOGI("LEDS", "LED ON");
}