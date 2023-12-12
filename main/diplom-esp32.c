#include "leds.h"
#include "string.h"
#include "nvs_flash.h"
#include "wifi_esp.h"
#include "mqtt_esp.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "dht11.h"

#define PIN_SENSOR 9

esp_mqtt_client_handle_t mqtt_client;

void app_main(void)
{
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    leds_init();

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_init_sta();

    ESP_ERROR_CHECK(esp_netif_init());

    mqtt_app_start(&mqtt_client);

    dht11_init(PIN_SENSOR, &mqtt_client);

    while (1){ ; }
}
