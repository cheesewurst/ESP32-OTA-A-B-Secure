#ifndef BOOT_COUNTER_H
#define BOOT_COUNTER_H

#include <stdint.h>
/**
 * @file boot_counter.h
 * @brief Header file for the Boot Counter component, which tracks the number of times the system has been booted. 
 *
 * @author Simon Schreurs
 * @date 2026-03-16
 * @version 1.0
 */

void boot_counter_init(void);
void boot_counter_increment(void);
uint32_t boot_counter_get(void);
void boot_counter_reset(void);

#endif // BOOT_COUNTER_H