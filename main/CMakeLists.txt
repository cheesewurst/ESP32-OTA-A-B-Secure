idf_component_register(SRCS "main.c" "version_info.c"
                       PRIV_REQUIRES nvs_flash spi_flash boot_counter ota_manager diagnostics_manager logger driver wifi_manager
                       INCLUDE_DIRS "")

                                     
