#include "diagnostics_manager.h"
#include "esp_log.h"
#include "../boot_counter/boot_counter.h"
#include "../ota_manager/ota_manager.h"

const char* TAG = "DIAGNOSTICS_MANAGER";

esp_err_t diagnostics_manager_run_firmware_check()
{
    esp_err_t err = diagnostics_manager_check_boot_count();
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Firmware check failed: %s", esp_err_to_name(err));
        return err;
    }

    return ESP_OK;
}

esp_err_t diagnostics_manager_check_boot_count()
{
    uint32_t boot_count = boot_counter_get();

    if(boot_count > 3) {
        return ESP_ERR_INVALID_STATE; // Indicate a potential boot loop
    }

    return ESP_OK;
}