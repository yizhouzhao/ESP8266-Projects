idf_component_register(SRCS "0-blink.c" "main.c"
                    INCLUDE_DIRS "."
                                "${CMAKE_CURRENT_LIST_DIR}/../managed_components/espressif__usb_device_uvc/tusb"
                    PRIV_REQUIRES nvs_flash esp_psram esp_http_server esp_wifi esp_timer)
