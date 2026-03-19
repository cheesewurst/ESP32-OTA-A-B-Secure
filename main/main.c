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
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "version_info.h"
#include "boot_counter.h"
#include "ota_manager.h"
#include "../components/config/OTA_CONFIG.h"
#include "esp_log.h"

static const char* TAG = "MAIN";

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
    while(1) {
        ota_manager_check_for_updates();
        vTaskDelay(OTA_CONFIG_CHECK_INTERVAL_MS / portTICK_PERIOD_MS);
    }
}

void start_ota_task(void) {
    ESP_LOGI(TAG, "Starting OTA task...");
    xTaskCreate(&ota_task, "ota_task", OTA_TASK_STACK_SIZE, NULL, 1, NULL);
}

void app_main(void)
{
    boot_counter_init();
    print_version_info();
    boot_counter_increment();

    print_boot_counter();
    boot_counter_reset();
    ota_manager_cleanup();
    start_ota_task();
}


