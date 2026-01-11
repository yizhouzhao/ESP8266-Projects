# ESP-Projects Copilot Instructions

## Project Overview
This workspace contains multiple independent ESP microcontroller projects, mixing ESP-IDF (CMake-based) and Arduino IDE frameworks. Each project resides in its own directory and is self-contained.

## Architecture
- **ESP-IDF Projects** (e.g., `0-blink/`, `1-wifi-cam/`, `y-esp32s3-webcam/`): Use CMake build system, FreeRTOS, ESP-IDF APIs
- **Arduino Projects** (e.g., `x-esp8266-plant/`): Use .ino files, Arduino libraries
- Each project has its own configuration (sdkconfig) and build artifacts

## Key Components
- `main/` directory contains application source code for ESP-IDF projects
- `managed_components/` for external ESP-IDF components (e.g., esp32-camera)
- `build/` directory generated during build process

## Developer Workflows
- **ESP-IDF Projects**: Use VS Code ESP-IDF extension for all operations
  - Build: Use extension's "Build" command (not manual `idf.py build`)
  - Flash: Use "Flash" command
  - Monitor: Use "Monitor" for serial output
  - Configure: Use "SDK Configuration Editor" for sdkconfig changes
- **Arduino Projects**: Use Arduino IDE or VS Code Arduino extension

## Conventions
- ESP-IDF apps use `app_main()` as entry point
- WiFi setup follows ESP-IDF event-driven pattern with event groups
- Camera projects use `esp_camera` component with pin configurations in `camera_pinout.h`
- HTTP servers use `esp_http_server` for web interfaces

## Examples
- Simple GPIO blink: [0-blink/main/0-blink.c](0-blink/main/0-blink.c)
- WiFi + HTTP server: [1-wifi-cam/main/1-wifi-cam.c](1-wifi-cam/main/1-wifi-cam.c)
- Camera with UVC: [y-esp32s3-webcam/main/main.c](y-esp32s3-webcam/main/main.c)
- Arduino sensor monitoring: [x-esp8266-plant/main.ino](x-esp8266-plant/main.ino)

## Integration Points
- External components via `managed_components/` (e.g., espressif__esp32-camera)
- WiFi connectivity using ESP-IDF WiFi APIs
- HTTP servers for web interfaces
- Sensor integration (DHT11 in Arduino projects)

Focus on using the ESP-IDF extension tools rather than manual commands for productivity.