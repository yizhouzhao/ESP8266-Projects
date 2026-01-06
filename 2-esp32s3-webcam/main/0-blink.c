#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

// Set the GPIO pin number for the LED
#define BLINK_GPIO 2 // Default for many ESP32 dev boards

void app_main(void)
{
    // Configure the GPIO pin
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    while (1) {
        printf("Turning on the LED\n");
        gpio_set_level(BLINK_GPIO, 1); // Set high
        vTaskDelay(); // Delay for 1 second

        printf("Turning off the LED\n");
        gpio_set_level(BLINK_GPIO, 0); // Set low
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }
}
