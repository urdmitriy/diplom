//
// Created by urdmi on 25.11.2023.
//

#ifndef DIPLOM_ESP32_DHT11_H
#define DIPLOM_ESP32_DHT11_H

typedef struct {
    int8_t temperature;
    uint8_t humidity;
} data_sensor_t;

void dht11_init(int pin_sensor);
void readSensors(data_sensor_t * data);

#endif //DIPLOM_ESP32_DHT11_H
