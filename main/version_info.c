#include "version_info.h"

void print_version_info(void)
{
    const version_info_t* info = get_version_info();
    printf("Firmware Version: %s\n", info->version);
    printf("Build Date: %s\n", info->build_date);
    printf("Git Hash: %s\n", info->git_hash);
}
const version_info_t* get_version_info(void)
{
    static version_info_t info = {
        .version = "1.0.0",
        .build_date = __DATE__ " " __TIME__,
        .git_hash = "unknown"
    };
    return &info;
}