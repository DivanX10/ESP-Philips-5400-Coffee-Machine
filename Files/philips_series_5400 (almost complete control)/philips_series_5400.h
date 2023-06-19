#pragma once

#include "esphome.h"
#include <stdarg.h>
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

#define POWER_STATE_TIMEOUT 500

// для рассчета кс
#define cs0 (uint8_t)crc
#define cs1 (uint8_t)(crc>>8)
#define cs2 (uint8_t)(crc>>16)
#define cs3 (uint8_t)(crc>>24)

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

            uint32_t crc; // буффер рассчета CRC
            void start_crc(uint8_t val); // начало рассчета CRC
            void add_crc(uint8_t val); //добавить байт к рассчету crc
            uint32_t calc_crc(); //закончить рассчет контрольной суммы
            uint32_t calc_crc(uint8_t* data, uint8_t size); // рассчет CRC буфера
            void send_packet(uint8_t* data, uint8_t size); // отправка пакета с рассчетом посторением к чертовой матери
            void _debugPrintPacket(uint8_t* data, uint8_t size, bool in);

        private:
            long last_message_from_mainboard_time_ = 0;
            /// @brief reference to uart connected to the display unit
            uart::UARTDevice display_uart_;
            /// @brief reference to uart connected to the mainboard
            uart::UARTDevice mainboard_uart_;
        };

    } // namespace philips_series_5400
} // namespace esphome
