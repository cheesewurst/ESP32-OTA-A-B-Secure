#ifndef OTA_MANAGER_H
#define OTA_MANAGER_H

#include <stdint.h>
#include "esp_err.h"
#include "esp_partition.h"

/**
 * @file ota_manager.h
 * @brief Header file for the OTA Manager component, which manages over-the-air updates for the firmware. 
 *
 * @author Simon Schreurs
 * @date 2026-03-16
 * @version 1.0
 */

typedef enum {
    OTA_STATUS_IDLE         = 0,
    OTA_STATUS_CHECKING,
    OTA_STATUS_DOWNLOADING,
    OTA_STATUS_UPDATING,
    OTA_STATUS_SUCCESS,
    OTA_STATUS_FAILED
} ota_update_status_t;

typedef struct {
    ota_update_status_t     status;
    uint32_t                last_update_time; // Timestamp of the last successful update
    const char*             current_version; // Current firmware version
} ota_manager_info_t;

void ota_manager_init(void);
esp_err_t ota_manager_check_for_updates(void);
esp_err_t ota_manager_perform_update(void);
esp_err_t ota_manager_check_version(const char* current_version, const char* new_version); // Check if the new version is valid and different from the current version
esp_err_t ota_manager_check_hash(const uint8_t* expected_hash, size_t hash_len); // Check the integrity of the downloaded firmware using a hash

esp_err_t ota_manager_find_inactive_partition(esp_partition_t** partition); // Find the inactive partition for the update
esp_err_t ota_manager_write_firmware_to_partition(esp_partition_t* partition); // Write the downloaded firmware to the inactive partition

esp_err_t ota_manager_set_update_https_url(const char* url); // Set the URL for OTA updates
esp_err_t ota_manager_set_update_interval(uint32_t interval_ms); // Set the interval for checking updates

esp_err_t ota_manager_get_update_status(uint32_t* update_status); // Get the current status of the OTA update process
esp_err_t ota_manager_get_last_update_time(uint32_t* last_interval_time); // Get the timestamp of the last successful update



#endif // OTA_MANAGER_H