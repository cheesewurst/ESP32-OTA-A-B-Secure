idf_component_register(
    SRCS "diagnostics_manager.c"
    INCLUDE_DIRS "."
    REQUIRES esp_partition esp_timer boot_counter logger nvs_flash
)