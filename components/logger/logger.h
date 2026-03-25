#ifndef LOGGER_H
#define LOGGER_H

#include "esp_err.h"

/**
 * @file logger.h
 * @brief Header file for the Logger component, which provides logging functionality for the firmware.
 *
 * @author Simon Schreurs
 * @date 2026-03-16
 * @version 1.0
 */

typedef enum {
    LOG_ERROR_NONE = 0,
    LOG_ERROR_OTA_PARTITION_MISSING,
    LOG_ERROR_OTA_DOWNLOAD_FAILED,
    LOG_ERROR_OTA_HASH_INVALID,
    LOG_ERROR_OTA_VERSION_INVALID,
    LOG_ERROR_BOOT_COUNT_EXCEEDED,
} log_error_t;

esp_err_t log_manager_save_error(log_error_t error);
esp_err_t log_manager_get_last_error(log_error_t* error);
esp_err_t log_manager_clear(void);

#endif // LOGGER_H