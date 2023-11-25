//
// Created by urdmi on 25.11.2023.
//

#include <stdio.h>
#include "dht11.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static int _pin_sensor;

void dht11_init(int pin_sensor) {
    _pin_sensor = pin_sensor;
    gpio_reset_pin(_pin_sensor);
}

void switch_sensor_pin_to_in (void ){
    gpio_set_direction(_pin_sensor, GPIO_MODE_INPUT);
}

void switch_sensor_pin_to_out (void ){
    gpio_set_direction(_pin_sensor, GPIO_MODE_OUTPUT);
}

uint8_t waitLine(uint8_t waitValue, uint16_t maxTime){
    unsigned long timeStart = micros();
    unsigned long length;
    while (digitalRead(pin)==waitValue && (length = micros()-timeStart)<maxTime);
    if (length > maxTime){
        return 1; //ошибка
    }
    return 0;
}

int8_t readWordFromSensor(uint8_t pin){
    int16_t data = 0;
    uint8_t error = 0;
    for (size_t i = 0; i < 16; i++)
    {
        waitLine(0,100); //ждем окончания "синхроимпульса"
        unsigned long startTime = micros(); //измеряем время имульса данных
        waitLine(1,100); //wait 1 or 0

        if (micros() - startTime > 40) { //Если импульс данных длиннее 40 (26-28) мкс, то бит = 1
            data |= (1 << (15-i));
        }
    }

    if (error == 0){
        return data>>8; //если ошибки не было, выдаем старший байт
    }else{
        ESP_LOGE("DHT11", "Error sensor");
        return 0;
    }
}

void readSensors(data_sensor_t * data){
    switch_sensor_pin_to_out();
    gpio_set_level(_pin_sensor, 0);
    vTaskDelay(pdMS_TO_TICKS(20)); // стартовый импульс
    gpio_set_level(_pin_sensor, 1);
    switch_sensor_pin_to_in();
    waitLine(1,100); // ждем начало импульса ответа
    waitLine(0,100); // ждем окончания импульса ответа
    waitLine(1,100); //  ждем начала передачи данных

    data->humidity =  readWordFromSensor(_pin_sensor);
    data->temperature = readWordFromSensor(_pin_sensor);
}
