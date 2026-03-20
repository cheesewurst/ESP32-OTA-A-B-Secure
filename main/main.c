/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "version_info.h"
#include "boot_counter.h"
#include "ota_manager.h"
#include "esp_log.h"
#include "diagnostics_manager.h"
#include "logger.h"
#include "../components/config/OTA_CONFIG.h"
#include "esp_ota_ops.h"
#include "wifi_manager.h"



#define LED_GPIO CONFIG_BLINK_GPIO

static const char* TAG = "MAIN";
static uint8_t s_led_state = 0;

void print_boot_counter(void)
{
    uint32_t boot_counter = boot_counter_get();
    if (boot_counter == 0) {
        printf("Boot Counter: Not initialized\n");
        return;
    }
    printf("Boot Counter: %" PRIu32 "\n", boot_counter);
}

void ota_task(void *pvParameter) {
    wifi_manager_init();
    while(1) {
        vTaskDelay(OTA_CONFIG_CHECK_INTERVAL_MS / portTICK_PERIOD_MS);
        ota_manager_check_for_updates();
    }
}

void blink_led(void* pvParameter) {
    while (1) {
        ESP_LOGI(TAG, "Blinking LED, state: %d", s_led_state);
        s_led_state = !s_led_state;
        vTaskDelay(CONFIG_BLINK_LED_DELAY_MS / portTICK_PERIOD_MS);
    }
}

void start_ota_task(void) {
    ESP_LOGI(TAG, "Starting OTA task...");
    xTaskCreate(&ota_task, "ota_task", OTA_CONFIG_TASK_STACK_SIZE, NULL, OTA_CONFIG_TASK_PRIORITY, NULL);
}

void start_app_task(void) {
    ESP_LOGI(TAG, "Starting main application task...");
    xTaskCreate(&blink_led, "blink_led", BLINK_LED_CONFIG_STACK_SIZE, NULL, BLINK_LED_CONFIG_TASK_PRIORITY, NULL);
}

void app_main(void)
{
    boot_counter_init();
    
    boot_counter_increment();
    print_boot_counter();

    esp_err_t diag_err = diagnostics_manager_run_firmware_check();
    if(diag_err != ESP_OK) {
            ESP_LOGE(TAG, "Firmware diagnostics failed: %s", esp_err_to_name(diag_err));
        ota_manager_rollback();
    }
    else {
        ota_manager_confirm_update();
        ESP_LOGI(TAG, "Update confirmed");
    }

    esp_err_t err = ota_manager_init();
    if(err != ESP_OK) {
        //Schrijf naar logger -> loggen dat OTA manager niet goed is geïnitialiseerd, en log de foutcode
        ESP_LOGE(TAG, "Failed to initialize OTA Manager: %s", esp_err_to_name(err));
        //todo: reset esp
        return;
    }

    boot_counter_reset();
    ota_manager_cleanup();

    print_version_info();
    start_ota_task();

    start_app_task();
}


