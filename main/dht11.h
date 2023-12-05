//
// Created by urdmi on 25.11.2023.
//

#ifndef DIPLOM_ESP32_DHT11_H
#define DIPLOM_ESP32_DHT11_H

#include "freertos/timers.h"

typedef struct {
    uint16_t humidity;
    uint16_t temperature;
    uint8_t crc;
} data_sensor_t;

typedef enum {
    DHT_FSM_START,
    DHT_FSM_ANSWER_IMPULSE_BEGIN,
    DHT_FSM_ANSWER_IMPULSE_END,
    DHT_FSM_BEGIN_DATA_RCV,
} fsm_dht_state_e;

static portMUX_TYPE my_spinlock = portMUX_INITIALIZER_UNLOCKED;;
static int _pin_sensor;

void vTaskDht11( void * pvParameters );
void dht11_init(int pin_sensor);
void dht11_start_task(void);
uint8_t dht11_wait_line(uint8_t waitValue);
void dht11_switch_pin_to_in (void );
void dht11_switch_pin_to_out (void );
void dht11_read(data_sensor_t* data);


#endif //DIPLOM_ESP32_DHT11_H
