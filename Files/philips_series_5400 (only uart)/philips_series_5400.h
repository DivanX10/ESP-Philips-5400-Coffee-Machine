#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#define POWER_STATE_TIMEOUT 500

namespace esphome
{
    namespace philips_series_5400
    {

        class PhilipsSeries5400 : public Component
        {
        public:
            void loop() override;
            void dump_config() override;

            void register_display_uart(uart::UARTComponent *uart) { display_uart_ = uart::UARTDevice(uart); };
            void register_mainboard_uart(uart::UARTComponent *uart) { mainboard_uart_ = uart::UARTDevice(uart); };

        private:
            long last_message_from_mainboard_time_ = 0;

            /// @brief reference to uart connected to the display unit
            uart::UARTDevice display_uart_;
            /// @brief reference to uart connected to the mainboard
            uart::UARTDevice mainboard_uart_;
        };

    } // namespace philips_series_5400
} // namespace esphome
