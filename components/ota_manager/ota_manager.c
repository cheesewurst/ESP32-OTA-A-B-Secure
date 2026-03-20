#include "ota_manager.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_https_ota.h"
#include "esp_http_client.h"
#include "esp_ota_ops.h"

#include "../../main/version_info.h"
#include "../config/OTA_CONFIG.h"
#include "../logger/logger.h"

#include <string.h>

static const char* TAG = "OTA_MANAGER";

ota_manager_info_t ota_manager_info;

extern const uint8_t server_cert_pem_start[] asm("_binary_ca_cert_pem_start");
extern const uint8_t server_cert_pem_end[]   asm("_binary_ca_cert_pem_end");

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
    //todo: - check if update is available http pull
    esp_err_t err = ota_manager_set_update_https_url(OTA_CONFIG_UPDATE_URL);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Invalid OTA URL: %s", esp_err_to_name(err));
        return err;
    }

    err = ota_manager_perform_update();
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "OTA update failed: %s", esp_err_to_name(err));
        return err;
    }
    return ESP_OK;
}

esp_err_t ota_manager_perform_update(void)
{
    esp_http_client_config_t http_config = {
        .url = ota_manager_info.update_url,
        .cert_pem = (char *)server_cert_pem_start, 
        .keep_alive_enable = true,
    };

    esp_https_ota_config_t ota_config = {
        .http_config = &http_config,
    };

    ota_manager_info.status = OTA_STATUS_DOWNLOADING;
    esp_err_t ret = esp_https_ota(&ota_config);

    if(ret == ESP_OK) {
        ota_manager_info.status = OTA_STATUS_SUCCESS;
        ESP_LOGI(TAG, "OTA Succeed, Rebooting...");
        esp_restart(); //Needs to be called somewhere else, not responsible for rebooting here
    } else {
        ota_manager_info.status = OTA_STATUS_FAILED;
        ESP_LOGE(TAG, "Firmware upgrade failed");
    }

    return ESP_OK;
}

esp_err_t ota_manager_cleanup(void)
{
    ota_manager_info.status = OTA_STATUS_IDLE;
    ota_manager_info.update_url[0] = '\0';
    return ESP_OK;
}

esp_err_t ota_manager_confirm_update(void)
{
    const esp_partition_t *running = esp_ota_get_running_partition();
    ESP_LOGI(TAG, "Confirming update for partition: %s", running->label);
    return esp_ota_mark_app_valid_cancel_rollback();
}

esp_err_t ota_manager_rollback(void)
{
    const esp_partition_t *running = esp_ota_get_running_partition();
    const esp_partition_t *other = esp_ota_get_next_update_partition(NULL);
    
    ESP_LOGI(TAG, "Rolling back from %s to %s", running->label, other->label);
    return esp_ota_mark_app_invalid_rollback_and_reboot();
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
    ESP_LOGW(TAG, "ota_manager_check_hash not implemented yet");
    return ESP_ERR_NOT_SUPPORTED;
} 

esp_err_t ota_manager_find_inactive_partition(esp_partition_t** partition)
{
    ESP_LOGW(TAG, "ota_manager_find_inactive_partition not implemented yet");
    return ESP_ERR_NOT_SUPPORTED;
} 

esp_err_t ota_manager_write_firmware_to_partition(esp_partition_t* partition)
{
    ESP_LOGW(TAG, "ota_manager_write_firmware_to_partition not implemented yet");
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t ota_manager_set_update_https_url(const char* url)
{
    if(url == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t err = ota_manager_check_https_url(url);
    if(err != ESP_OK) {
        return err;
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
