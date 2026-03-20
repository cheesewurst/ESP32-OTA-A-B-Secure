idf_component_register(SRCS "main.c" "version_info.c"
                       PRIV_REQUIRES spi_flash boot_counter ota_manager diagnostics_manager logger driver
                       INCLUDE_DIRS "")
