#include <stdio.h>
#include "leds.h"
#include "string.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "wifi_esp.h"
#include "mqtt_esp.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "dht11.h"

#define PIN_SENSOR 9

void app_main(void)
{

    leds_init();
    dht11_init(PIN_SENSOR);


//    //Initialize NVS
//    esp_err_t ret = nvs_flash_init();
//    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
//        ESP_ERROR_CHECK(nvs_flash_erase());
//        ret = nvs_flash_init();
//    }
//    ESP_ERROR_CHECK(ret);
//
//    wifi_init_sta();
//
//    esp_log_level_set("*", ESP_LOG_INFO);
//    esp_log_level_set("mqtt_client", ESP_LOG_VERBOSE);
//    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
//    esp_log_level_set("TRANSPORT_BASE", ESP_LOG_VERBOSE);
//    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
//    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
//    esp_log_level_set("outbox", ESP_LOG_VERBOSE);
//
//    ESP_ERROR_CHECK(esp_netif_init());
//
//    mqtt_app_start();
//    dht11_init(PIN_SENSOR);

    while (1){

    }
}
