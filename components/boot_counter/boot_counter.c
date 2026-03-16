#include "boot_counter.h"
#include "nvs_flash.h"

void boot_counter_init(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }
}

void boot_counter_increment(void)
{
    nvs_handle_t nvs_handle;
    uint32_t boot_count = 0;

    esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) return;

    nvs_get_u32(nvs_handle, "boot_count", &boot_count);
    boot_count++;
    nvs_set_u32(nvs_handle, "boot_count", boot_count);
    nvs_commit(nvs_handle);
    nvs_close(nvs_handle);
}

uint32_t boot_counter_get(void)
{
    nvs_handle_t nvs_handle;
    uint32_t boot_count;

    esp_err_t err = nvs_open("storage", NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) return 0;

    nvs_get_u32(nvs_handle, "boot_count", &boot_count);
    nvs_close(nvs_handle);

    return boot_count;
}

void boot_counter_reset(void)
{
    nvs_handle_t nvs_handle;
    uint32_t boot_count = 0;

    esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) return;

    nvs_set_u32(nvs_handle, "boot_count", boot_count);
    nvs_commit(nvs_handle);
    nvs_close(nvs_handle);
}
