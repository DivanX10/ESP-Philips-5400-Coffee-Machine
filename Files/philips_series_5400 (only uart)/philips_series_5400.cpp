#include "esphome/core/log.h"
#include "philips_series_5400.h"

#define BUFFER_SIZE 32

namespace esphome {
namespace philips_series_5400 {

static const char *TAG = "philips_series_5400";

void PhilipsSeries5400::loop() {
    uint8_t buffer[BUFFER_SIZE];

    // Pipe display to mainboard
    if (display_uart_.available()) {
        uint8_t size = std::min(display_uart_.available(), BUFFER_SIZE);
        display_uart_.read_array(buffer, size);

        mainboard_uart_.write_array(buffer, size);
    }

    // Pipe to display
    if (mainboard_uart_.available()) {
        uint8_t size = std::min(mainboard_uart_.available(), BUFFER_SIZE);
        mainboard_uart_.read_array(buffer, size);

        display_uart_.write_array(buffer, size);
    }
}

void PhilipsSeries5400::dump_config() {
    ESP_LOGCONFIG(TAG, "Philips Series 5400");
    display_uart_.check_uart_settings(115200, 1, uart::UART_CONFIG_PARITY_NONE, 8);
    mainboard_uart_.check_uart_settings(115200, 1, uart::UART_CONFIG_PARITY_NONE, 8);
}

} // namespace philips_series_5400
} // namespace esphome
