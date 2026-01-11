idf_component_register(SRCS "1-wifi-cam.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES esp_http_server esp_wifi nvs_flash)
