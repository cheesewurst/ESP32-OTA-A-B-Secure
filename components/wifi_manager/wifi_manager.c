#include "wifi_manager.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_log.h"

#define WIFI_SSID "Simon's iPhone"
#define WIFI_PASSWORD "sauskaas"

esp_err_t wifi_manager_init(void) {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
        },
    };
    
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_err_t err = esp_wifi_start();
    if(err != ESP_OK)
    {
        ESP_LOGE("WIFI_MANAGER", "Wifi failed to start");
        return err;
    }
    err = esp_wifi_connect();
    if(err != ESP_OK)
    {
        ESP_LOGE("WIFI_MANAGER", "Wifi failed to connect");
        return err;
    }

    return ESP_OK;
}