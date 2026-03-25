idf_component_register(
    SRCS "ota_manager.c"
    INCLUDE_DIRS "."
    REQUIRES esp_https_ota app_update nvs_flash esp_partition
)
target_add_binary_data(${COMPONENT_LIB} "ca_cert.pem" TEXT)         