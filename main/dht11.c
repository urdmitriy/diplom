//
// Created by urdmi on 25.11.2023.
//

#include <stdio.h>
#include "leds.h"
#include "dht11.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_log.h"

void vTaskDht11( void * pvParameters )
{
    data_sensor_t data_sensor = {.humidity = 255, .temperature = 255};
    ESP_LOGI("dht11", "Temperature = %d, humidity = %d", data_sensor.temperature, data_sensor.humidity);

    for( ;; )
    {
        ESP_LOGI("dht11", "Start task DHT11");
        leds_flash(LED_YELLOW, 100);
        dht11_switch_pin_to_out();
        gpio_set_level(_pin_sensor, 0);
        vTaskDelay(pdMS_TO_TICKS(20)); // стартовый импульс
        gpio_set_level(_pin_sensor, 1);
        dht11_switch_pin_to_in();
        dht11_wait_line(1); // ждем начало импульса ответа
        dht11_wait_line(0); // ждем окончания импульса ответа
        dht11_wait_line(1); //  ждем начала передачи данных

        data_sensor.humidity =  dht11_read_word();
        data_sensor.temperature = (int8_t)dht11_read_word();
        ESP_LOGI("dht11", "Temperature = %d, humidity = %d", data_sensor.temperature, data_sensor.humidity);
        ESP_LOGI("dht11", "END task DHT11");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void dht11_init(int pin_sensor) {
    _pin_sensor = pin_sensor;
    gpio_reset_pin(_pin_sensor);
    ESP_LOGI("DHT", "Pis sensor set to %d", _pin_sensor);

    xTaskCreate( vTaskDht11, "DHT11", 8096, NULL, 1, NULL );
}

void dht11_switch_pin_to_in (void ){
    gpio_set_direction(_pin_sensor, GPIO_MODE_INPUT);
}

void dht11_switch_pin_to_out (void ){
    gpio_set_direction(_pin_sensor, GPIO_MODE_OUTPUT);
}

uint8_t dht11_wait_line(uint8_t waitValue){

    while (gpio_get_level(_pin_sensor) == waitValue);
    return 0;
}

uint8_t dht11_read_word(void ){
    uint16_t data = 0;
    TickType_t time_start = pdTICKS_TO_MS(xTaskGetTickCount());

    for (size_t i = 0; i < 16; i++)
    {
        dht11_wait_line(0); //ждем окончания "синхроимпульса"
        dht11_wait_line(1); //wait 1 or 0

        if (pdTICKS_TO_MS(xTaskGetTickCount()) - time_start > 40) { //Если импульс данных длиннее 40 (26-28) мкс, то бит = 1
            data |= (1 << (15-i));
        }

    }
    return (data >> 8); //если ошибки не было, выдаем старший байт

}

void dht11_read(data_sensor_t * data){
    dht11_switch_pin_to_out();
    gpio_set_level(_pin_sensor, 0);
    vTaskDelay(pdMS_TO_TICKS(20)); // стартовый импульс
    gpio_set_level(_pin_sensor, 1);
    dht11_switch_pin_to_in();
    dht11_wait_line(1); // ждем начало импульса ответа
    dht11_wait_line(0); // ждем окончания импульса ответа
    dht11_wait_line(1); //  ждем начала передачи данных

    data->humidity = dht11_read_word();
    data->temperature = dht11_read_word();
}
