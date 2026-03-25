idf_component_register(
    SRCS "boot_counter.c"
    INCLUDE_DIRS "."
    REQUIRES nvs_flash 
)