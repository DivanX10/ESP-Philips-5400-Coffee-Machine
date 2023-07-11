#pragma once

#ifdef ESP32
   #include "esphome/core/preferences.h"
#else
   # ERROR: No-no-nooy! Friend, it only for ESP32
#endif

#include "esphome.h"
#include <stdarg.h>
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/number/number.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/select/select.h"
#include "esphome/components/button/button.h"

#define POWER_STATE_TIMEOUT 500

static const char *TAG = "Philips-5400 ";

// структура для сохранения данных
struct save_data_t {
    uint16_t volume=0;
    uint16_t milk=0;
    uint8_t grind=0;
    uint8_t cups=0;
    uint8_t product=0;
    uint8_t free=0;
};

namespace esphome
{
    namespace philips_series_5400
    {
        using namespace esphome::sensor;
        using namespace esphome::text_sensor;
        using namespace esphome::number;
        using namespace esphome::select;
        using namespace esphome::button;

        class PhilipsSeries5400;

        class PhilipsSeries5400Button : public esphome::button::Button, public esphome::Parented<PhilipsSeries5400> {
           protected:
           void press_action() override{
              //..
           }
           friend class PhilipsSeries5400;   
        };

        class PhilipsSeries5400Select : public esphome::select::Select, public esphome::Parented<PhilipsSeries5400> {
            protected:
            void control(const std::string &value) override{
                this->state_callback_.call(value, 0);
            }
            friend class PhilipsSeries5400;   
        };

        class PhilipsSeries5400Number : public esphome::number::Number, public esphome::Parented<PhilipsSeries5400> {
            protected:
            void control(float value) override{
                this->state_callback_.call(value);
            }
            friend class PhilipsSeries5400;   
        };

        class PhilipsSeries5400 : public Sensor, public esphome::Component
        {
        public:
            void loop() override;
            void dump_config() override;
            void setup()override;
            void register_display_uart(uart::UARTComponent *uart) { display_uart_ = uart::UARTDevice(uart);}
            void register_mainboard_uart(uart::UARTComponent *uart) { mainboard_uart_ = uart::UARTDevice(uart);}
            void register_work_sensor(sensor::Sensor *sens){ work_sensor_ = sens;} // установка сенсора
            void set_sens_b0(TextSensor *sens){ sens_b0 = sens;} 
            void set_sens_b5(TextSensor *sens){ sens_b5 = sens;} 
            void set_sens_ba(TextSensor *sens){ sens_ba = sens;} 
            void set_sens_90(TextSensor *sens){ sens_90 = sens;} 
            void set_sens_91(TextSensor *sens){ sens_91 = sens;} 
            void set_sens_93(TextSensor *sens){ sens_93 = sens;} 
            void set_sens_product(TextSensor *sens){ sens_product = sens;} 
            void set_vol_number(PhilipsSeries5400Number *num){ // объем кофе
               volume = num;
               volume->add_on_state_callback([this](float sensor_value) {
                  if (!std::isnan(sensor_value)) {
                     _volume=sensor_value;
                     //ESP_LOGD(TAG,"Set coffee volume to %d", _volume);
                     coffee_test_validate();
                  }
               });
               if(isnan(volume->state)){
                  _volume=volume->traits.get_min_value();
               } else {
                  _volume=volume->state;
                  if(_volume<volume->traits.get_min_value()){
                     _volume=volume->traits.get_min_value();
                  } else if(_volume>volume->traits.get_max_value()){
                     _volume=volume->traits.get_max_value();
                  }
               }
               volume->publish_state(_volume);
            }   
            void set_milk_number(PhilipsSeries5400Number *num){  // объем молока
               milk = num;
               milk->add_on_state_callback([this](float sensor_value) {
                  if (!std::isnan(sensor_value)) {
                     _milk=sensor_value;
                     //ESP_LOGD(TAG,"Set milk to %d", _milk);
                     coffee_test_validate();
                  }
               });
               if(isnan(milk->state)){
                  _milk=milk->traits.get_min_value();
               } else {
                  _milk=milk->state;
                  if(_milk<milk->traits.get_min_value()){
                     _milk=milk->traits.get_min_value();
                  } else if(_volume>milk->traits.get_max_value()){
                     _milk=milk->traits.get_max_value();
                  }
               }
               milk->publish_state(_milk);
            }   
            void set_grind_select(PhilipsSeries5400Select *sel){  // помол/крепость
               grind = sel;
               grind->add_on_state_callback([this](std::string payload, size_t num) {
                  _grind = grind->index_of(payload).value();
                  //ESP_LOGD(TAG,"Set grind to %d", _grind);
                  coffee_test_validate();
               });
               _grind=grind->active_index().has_value();
               if(isnan(_grind)){
                  _grind=0;
               }
               grind->publish_state(grind->at(_grind).value());
            }
            void set_cups_select(PhilipsSeries5400Select *sel){  // количество чашек
               cups = sel;
               cups->add_on_state_callback([this](std::string payload, size_t num) {
                  _cups = cups->index_of(payload).value();
                  //ESP_LOGD(TAG,"Set cups to %d", _cups);
                  coffee_test_validate();
               });
               _cups=cups->active_index().has_value();
               if(isnan(_cups)){
                  _cups=0;
               }
               cups->publish_state(cups->at(_cups).value());
            }
            void set_product_select(PhilipsSeries5400Select *sel){  // напиток
               product = sel;
               product->add_on_state_callback([this](std::string payload, size_t num) {
                  _product = product->index_of(payload).value();
                  //ESP_LOGD(TAG,"Set product to %s (%d)", payload.c_str(), _product);
                  coffee_test_validate();
               });
               _product=product->active_index().has_value();
               if(isnan(_product)){
                  _product=0;
               }
               product->publish_state(product->at(_product).value());
            }
            void set_start_button(PhilipsSeries5400Button *key){ // кнопка запуска приготовления кофе
               start_button = key;
               start_button->add_on_press_callback([this](void){
                  coffee_build(_product, _grind, _cups, _volume, _milk); //запускаем кофе
                  coffe_route=1; // запуск приготовления кофе
               });
            }
            void set_store_settings(bool store) {_need_store = store;} // установка флага сохранения данных
            void set_debug_settings(bool val) {_debug = val;} // установка флага отладки
            void prepare(uint8_t* data){ //безусловный старт приготовления кофе пакетом
               coffee_start(data);
               coffe_route=1; // установить флаг исполнения
            }
            void send_packet_main(uint8_t* data, uint8_t size); // отправка пакета с рассчетом посторением к чертовой матери
            void send_packet_displ(uint8_t* data, uint8_t size); // отправка пакета с рассчетом посторением дисплею
        private:
            uart::UARTDevice display_uart_;
            uart::UARTDevice mainboard_uart_;
            void pubMess(TextSensor *sens,uint8_t* buff, uint8_t size);
            void pubStat(uint8_t stat);
            sensor::Sensor *work_sensor_{nullptr};
            TextSensor *sens_90{nullptr};
            TextSensor *sens_91{nullptr};
            TextSensor *sens_93{nullptr};
            TextSensor *sens_b0{nullptr};
            TextSensor *sens_b5{nullptr};
            TextSensor *sens_ba{nullptr};
            PhilipsSeries5400Number *volume{nullptr};   // объем кофе
            uint16_t _volume=0;
            PhilipsSeries5400Number *milk{nullptr};   // объем молока
            uint16_t _milk=0;
            PhilipsSeries5400Select *grind{nullptr};   // помол/крепость
            uint8_t _grind=0;
            PhilipsSeries5400Select *cups{nullptr};   // количество чашек
            uint8_t _cups=0;
            PhilipsSeries5400Select *product{nullptr};   // тип напитка
            uint8_t _product=0;
            PhilipsSeries5400Button *start_button{nullptr}; // кнопка "приготовить кофе"
            uint8_t coffe_route=0; //для роутинга приготовления кофе
            TextSensor *sens_product{nullptr}; // текстовый сенсор готовящегося напитка
            save_data_t stored_data; // буфер для сохранения данных
            bool _need_store=false; // флаг необходимости сохранения данных
            bool _debug=false; // флаг режима отладки
            ESPPreferenceObject storage = global_preferences->make_preference<save_data_t>(this->get_object_id_hash(), true);
            void checkStore();// проверка и при необходимости сохранение данных
            void reStore();// восстановлениеданных из флеша
            inline void start_crc(uint8_t val); // начало рассчета CRC
            inline void add_crc(uint8_t val); //добавить байт к рассчету crc
            inline uint32_t calc_crc(); //закончить рассчет контрольной суммы
            inline uint32_t calc_crc(uint8_t* data, uint8_t size); // рассчет CRC буфера
            void send_packet_9X(uint8_t* data, uint8_t size); // отправка 90x пакетов с подменой нумерации
            void send_packet_90(uint8_t* data, uint8_t size);// отправка 90x пакетов из конфига
            void send_packet_FF(uint8_t* data, uint8_t size); // отправка пакета FF в ответ, он связан с 9X
            void coffee_test_validate(); // показ построения рецепта в тестовом режиме
            void _debugPrintPacket(uint8_t* data, uint8_t size, bool in);
            void coffee_start(uint8_t* data); // старт приготовления кофе пакетом
            // построение рецепта кофе в буфере данных и запуск
            // type - вид напитка
            // cups - количество чашек
            // bean - крепкость от 2 до 0, 3- использовать мототый
            // vol - объем кофе в мл
            // milk - объем молока в мл
            void coffee_build(uint8_t type, uint8_t bean, uint8_t cups, uint16_t vol, uint16_t milk);
            // расшифровка рецепта в цифры
            bool coffee_decode(const uint8_t* rec, uint8_t* type, uint8_t* bean, uint8_t* cups, uint16_t* vol, uint16_t* milk); 
            // текстовая расшифровка продукта в работе 
            void coffee_show(uint8_t* data);
            friend class PhilipsSeries5400Button;
            friend class PhilipsSeries5400Select;
            friend class PhilipsSeries5400Number;
        };
    } // namespace philips_series_5400
} // namespace esphome
