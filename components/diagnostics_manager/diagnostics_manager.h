#ifndef DIAGNOSTICS_RUNNER_H
#define DIAGNOSTICS_RUNNER_H

#include "esp_err.h"

/**
 * @file diagnostics_runner.h
 * @brief Header file for the Diagnostics Runner component, which checks new firmware updates before performing the OTA update. 
 * It ensures that the new firmware is valid and compatible with the device, and can perform various checks such as version comparison, hash verification, and partition validation.
 *
 * @author Simon Schreurs
 * @date 2026-03-16
 * @version 1.0
 */

esp_err_t diagnostics_runner_check_version();

#endif // DIAGNOSTICS_RUNNER_H