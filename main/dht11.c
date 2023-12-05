//
// Created by urdmi on 25.11.2023.
//

#include <stdio.h>
#include "leds.h"
#include "mqtt_esp.h"
#include "dht11.h"
#include "driver/gpio.h"
#include "driver/timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_log.h"

void dht11_init(int pin_sensor) {
    _pin_sensor = pin_sensor;
    gpio_reset_pin(_pin_sensor);
    ESP_LOGI("DHT", "Pis sensor set to %d", _pin_sensor);
    timer_config_t config_timer = {
            .counter_dir = TIMER_COUNT_UP,
            .divider = 40,
            .auto_reload = TIMER_AUTORELOAD_EN,
            .alarm_en = TIMER_ALARM_DIS,
            .counter_en = TIMER_START};
    timer_init(TIMER_GROUP_0, TIMER_0, &config_timer);
}

void dht11_start_task(void) {
    xTaskCreate( vTaskDht11, "DHT11", 8096, NULL, 1, NULL );
}

void dht11_switch_pin_to_in (void ){
    gpio_set_direction(_pin_sensor, GPIO_MODE_INPUT);
}

void dht11_switch_pin_to_out (void ){
    gpio_set_direction(_pin_sensor, GPIO_MODE_OUTPUT);
}

void vTaskDht11( void * pvParameters )
{
    data_sensor_t data_sensor = {.humidity = 127, .temperature = 127};
    fsm_dht_state_e dht_state = DHT_FSM_START;
    for( ;; )
    {
        switch (dht_state) {
            case DHT_FSM_START:
                ESP_LOGI("dht11", "Start task DHT11");
                leds_flash(LED_YELLOW, 100);
                dht11_switch_pin_to_out();
                gpio_set_level(_pin_sensor, 0);
                vTaskDelay(pdMS_TO_TICKS(20)); // стартовый импульс
                gpio_set_level(_pin_sensor, 1);
                dht11_switch_pin_to_in();
                dht_state = DHT_FSM_ANSWER_IMPULSE_BEGIN;
                ESP_LOGI("dht11", "dht_state = DHT_FSM_ANSWER_IMPULSE_BEGIN");
                break;
            case DHT_FSM_ANSWER_IMPULSE_BEGIN:
                if (gpio_get_level(_pin_sensor) == 1) {
                    dht_state = DHT_FSM_ANSWER_IMPULSE_END;
                    ESP_LOGI("dht11", "dht_state = DHT_FSM_ANSWER_IMPULSE_END");
                }
                break;
            case DHT_FSM_ANSWER_IMPULSE_END:
                if (gpio_get_level(_pin_sensor) == 0) {
                    dht_state = DHT_FSM_BEGIN_DATA_RCV;
                    ESP_LOGI("dht11", "dht_state = DHT_FSM_BEGIN_DATA_RCV");
                }
                break;
            case DHT_FSM_BEGIN_DATA_RCV:
                dht11_read(&data_sensor);
                ESP_LOGI("test", "Temperature = %d, h = %d, crc = %d", data_sensor.temperature, data_sensor.humidity, data_sensor.crc);
                //esp_mqtt_client_publish(_mqtt_client, "urdmitriy/data", data_sensor, 0, 1, 0);
                dht_state = DHT_FSM_START;
                vTaskDelay(pdMS_TO_TICKS(2000));
                break;
        }
    }
}

void dht11_read(data_sensor_t* data){
    taskENTER_CRITICAL(&my_spinlock);
    uint64_t time_start, time_current;
    timer_get_counter_value(TIMER_GROUP_0, TIMER_0, &time_start);
//        dht11_wait_line(0); //ждем окончания "синхроимпульса"
//        dht11_wait_line(1); //wait 1 or 0
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 8; ++j) {
            timer_get_counter_value(TIMER_GROUP_0, TIMER_0, &time_current);
            if (time_current - time_start > 40) { //Если импульс данных длиннее 40 (26-28) мкс, то бит = 1
                *(((uint8_t *) data) + i) |= (uint8_t) (1 << (7 - j));
            }
        }
    }

    taskEXIT_CRITICAL(&my_spinlock);
}
