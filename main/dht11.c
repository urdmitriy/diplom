//
// Created by urdmi on 25.11.2023.
//

#include <stdio.h>
#include "mqtt_esp.h"
#include "dht11.h"
#include "driver/gpio.h"
#include "driver/timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

void dht11_init(int pin_sensor, esp_mqtt_client_handle_t* mqtt_client) {
    _pin_sensor = pin_sensor;
    _mqtt_client = mqtt_client;
    gpio_reset_pin(_pin_sensor);
    timer_config_t config_timer = {
            .counter_dir = TIMER_COUNT_UP,
            .divider = 80,
            .auto_reload = TIMER_AUTORELOAD_EN,
            .alarm_en = TIMER_ALARM_DIS,
            .counter_en = TIMER_START};
    timer_init(TIMER_GROUP_0, TIMER_0, &config_timer);
}

void dht11_start_task(void) {
    xTaskCreate(dht11_vTask_read, "DHT11", 8096, NULL, 1, NULL);
}

void dht11_vTask_read(void * pvParameters )
{
    data_sensor_t data_sensor = {.humidity = 0, .temperature = 0, .crc = 0};

    for( ;; )
    {
        dht11_read(&data_sensor);
        uint8_t crc_calc =
                (uint8_t)(data_sensor.temperature>>8)+
                (uint8_t)(data_sensor.temperature & 0x00FF) +
                (uint8_t)(data_sensor.humidity>>8);
        ESP_LOGI("DHT11", "Temperature: %d.%d, humidity: %d.%d, crc: %d, crc calc: %d.",
                 data_sensor.temperature>>8, data_sensor.temperature & 0x00FF, data_sensor.humidity>>8,
                 data_sensor.humidity & 0x00FF, data_sensor.crc, crc_calc);

        char message[500];

        sprintf(message, "<temperature>%d.%d</temperature>,<humidity>%d.%d</humidity>",
                data_sensor.temperature>>8, (uint8_t)(data_sensor.temperature & 0x00FF),
                data_sensor.humidity>>8, (uint8_t)(data_sensor.humidity & 0x00FF));

        esp_mqtt_client_publish(*_mqtt_client, "urdmitriy/data",  message, 0, 1, 0);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void dht11_read(data_sensor_t* data) {

    uint64_t time_start, time_stop, time_delta;
    portMUX_TYPE my_spinlock = portMUX_INITIALIZER_UNLOCKED;

    data->humidity = 0;
    data->temperature = 0;
    data->crc = 0;

    gpio_set_direction(_pin_sensor, GPIO_MODE_OUTPUT);
    gpio_set_level(_pin_sensor, 0);
    vTaskDelay(pdMS_TO_TICKS(20));
    gpio_set_level(_pin_sensor, 1);
    gpio_set_direction(_pin_sensor, GPIO_MODE_INPUT);

    taskENTER_CRITICAL(&my_spinlock);

    while (gpio_get_level(_pin_sensor) == 1); //answer begin
    while (gpio_get_level(_pin_sensor) == 0); //Answer end, DHT pull up voltage
    while (gpio_get_level(_pin_sensor) == 1); //Data begin
    while (gpio_get_level(_pin_sensor) == 0); //First bit begin

    for (int i = 0; i <= 4; i+=2) {
        for (int j = 2; j > 0 ; --j) {
            for (int k = 0; k < 8; ++k) {
                timer_get_counter_value(TIMER_GROUP_0, TIMER_0, &time_start);
                while (gpio_get_level(_pin_sensor) == 1);
                timer_get_counter_value(TIMER_GROUP_0, TIMER_0, &time_stop);
                time_delta = time_stop - time_start;

                if (time_delta > 40) {
                    *(((uint8_t *) data) + i + j - 1) |= (1 << (7 - k));
                }
                if (i + j == 6) {
                    taskEXIT_CRITICAL(&my_spinlock);
                    return;
                }
                while (gpio_get_level(_pin_sensor) == 0);
            }
        }
    }
    taskEXIT_CRITICAL(&my_spinlock);
}