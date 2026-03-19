#include "ota_manager.h"
#include "esp_log.h"
#include "esp_system.h"

#include "../../main/version_info.h"
#include "../config/OTA_CONFIG.h"
#include "../logger/logger.h"

#include <string.h>

static const char* TAG = "OTA_MANAGER";

ota_manager_info_t ota_manager_info;

esp_err_t ota_manager_init(void)
{
    const esp_partition_t *ota_0 = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_0, NULL);
    const esp_partition_t *ota_1 = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_1, NULL);

    if (ota_0 == NULL || ota_1 == NULL) {
        ESP_LOGE(TAG, "OTA partitions not found, check partition table");
        return ESP_ERR_NOT_FOUND;
    }

    ota_manager_info.status = OTA_STATUS_IDLE;
    return ESP_OK;
}

esp_err_t ota_manager_check_for_updates(void)
{
    ESP_LOGI(TAG, "Checking for updates...");
    return ESP_OK;
}

esp_err_t ota_manager_perform_update(void)
{
    return ESP_OK;
}

esp_err_t ota_manager_cleanup(void)
{
    return ESP_OK;
}

esp_err_t ota_manager_confirm_update(void)
{
    return ESP_OK;
}

esp_err_t ota_manager_rollback(void)
{
    return ESP_OK;
}


esp_err_t ota_manager_check_version(const char* new_version)
{
    if(new_version == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    if(strcmp(ota_manager_info.current_version, new_version) < 0) {
        return ESP_OK; 
    } 
    return ESP_ERR_INVALID_VERSION; 
} 

esp_err_t ota_manager_check_https_url(const char* url)
{
    if(url == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    if(strncmp(url, "https://", 8) == 0) {
        return ESP_OK; 
    } 

    return ESP_ERR_INVALID_VERSION; 
}

esp_err_t ota_manager_check_hash(const uint8_t* expected_hash, size_t hash_len)
{
    return ESP_OK;
} 

esp_err_t ota_manager_find_inactive_partition(esp_partition_t** partition)
{
    return ESP_OK;
} 

esp_err_t ota_manager_write_firmware_to_partition(esp_partition_t* partition)
{
    return ESP_OK;
} 

esp_err_t ota_manager_set_update_https_url(const char* url)
{
    if(url == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    strncpy(ota_manager_info.update_url, url, sizeof(ota_manager_info.update_url) - 1);
    ota_manager_info.update_url[sizeof(ota_manager_info.update_url) - 1] = '\0';
    return ESP_OK;
}

esp_err_t ota_manager_set_update_interval(uint32_t interval_ms)
{
    if(interval_ms == 0) {
        return ESP_ERR_INVALID_ARG;
    }
    ota_manager_info.last_update_time = interval_ms;
    return ESP_OK;
}

esp_err_t ota_manager_get_update_status(ota_update_status_t* update_status)
{
    if(update_status == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    *update_status = ota_manager_info.status;
    return ESP_OK;
}

esp_err_t ota_manager_get_last_update_time(uint32_t* last_interval_time)
{
    if(last_interval_time == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    *last_interval_time = ota_manager_info.last_update_time;
    return ESP_OK;
} 
