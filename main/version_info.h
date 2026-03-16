#ifndef VERSION_INFO_H
#define VERSION_INFO_H

#include <stdio.h>

/**
 * @file boot_counter.h
 * @brief Header file for the Version Info component, which tracks the version info of the firmware. 
 *
 * @author Simon Schreurs
 * @date 2026-03-16
 * @version 1.0
 */

typedef struct {
    const char* version;
    const char* build_date;
    const char* git_hash;
} version_info_t;

void print_version_info(void);
const version_info_t* get_version_info(void);

#endif