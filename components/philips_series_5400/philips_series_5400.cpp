#include <Arduino.h>
#include <stdarg.h>
#include "esphome/core/log.h"
#include "esphome/core/log.h"
#include "philips_series_5400.h"

#define BUFFER_SIZE 256

//to calculate the checksum
#define POLY 0x04c11db7

namespace esphome {
namespace philips_series_5400 {

static const char *TAG = "philips_series_5400";

uint32_t crc = 0xffffffff; //CRC calculation buffer

void PhilipsSeries5400::start_crc(uint8_t val){
   crc = 0xffffffff; //clearing the buffer for the next calculation
   add_crc(val);
}
 
//add a byte to the crc calculation
void PhilipsSeries5400::add_crc(uint8_t val){
    uint8_t t8 = 0;
    for (uint8_t j = 0; j < 8; j++) {
       if (val & (1 << j)) {
          t8 |= (uint8_t)(1 << (7 - j));
       }
    }
    crc ^= (uint32_t)t8 << 24;
    for (uint8_t j = 0; j < 8; j++) {
      if (crc & 0x80000000) {
        crc = (uint32_t)((crc << 1) ^ POLY);
      } else {
        crc <<= 1;
      }
    }
}

//finish calculating the checksum
uint32_t PhilipsSeries5400::calc_crc(){ 
    uint32_t t32 = 0;
    for (uint8_t i = 0; i < 32; i++) {
       if (crc & (1U << i)) {
           t32 |= (uint32_t)(1 << (31 - i));
       }
    }
    crc = t32 ^ 0xffffffff;
    return crc;
}


//calculation of the CRC buffer
uint32_t PhilipsSeries5400::calc_crc(uint8_t* data, uint8_t size){
   start_crc(data[0]);
   for(uint8_t i=1; i<size; i++){
      add_crc(data[i]);
   } 
   return calc_crc();  
}

void PhilipsSeries5400::_debugPrintPacket(uint8_t* data, uint8_t size, bool in){
   String st = "";
   char textBuf[11];
   //we fill in the time of receiving the package
   memset(textBuf, 0, 11);
   sprintf(textBuf, "%010u", millis());
   st = st + textBuf + ": ";
   //forming the preamble
   if (in) {
      st += "[<=] ";      //indication of an incoming packet
   } else {
      st += "[=>] ";      //flag of an outgoing packet
   } 
   for (uint8_t i=0; i<size; i++){
      memset(textBuf, 0, 11);
      sprintf(textBuf, " %02X", data[i]);
      st += textBuf;
   }
   ESP_LOGE("",st.c_str());
}


//sending a package with a calculation
void PhilipsSeries5400::send_packet(uint8_t* data, uint8_t size){
    uint8_t pmb[]={0xAA,0xAA,0xAA}; //preamble
    uint8_t post=0x55; //postambula
    uint8_t cs[]={0,0,0,0}; //checksum buffer
    //if(data[0]==0x91 || data[0]==0x90 || data[0]==0x93){ //for these packages, we replace the end-to-end counter 
    //   ESP_LOGE("","COUNTER IN: %d",data[1]);
    //   static uint8_t counter=data[1];
    //   if(data[3]==0x0F){
    //      counter=data[1];
    //   }
    //   data[1]=counter; //auto package counter
    //   ESP_LOGE("","COUNTER OUT: %d",data[1]);
    //   counter++;
    //   if(counter>0x2B){
    //      counter=6;
    //   }
    //}
    //_debugPrintPacket(data, size, true);
    *((uint32_t*)cs)=calc_crc(data,size);
    mainboard_uart_.write_array(pmb, sizeof(pmb));
    mainboard_uart_.write_array(data, size);
    mainboard_uart_.write_array(cs, sizeof(cs));
    mainboard_uart_.write_array(&post,1);
}

void PhilipsSeries5400::loop() {

    uint8_t temp; //buffer for reading a byte from a port
    //Pipe display to mainboard
    static uint8_t buffer_board[BUFFER_SIZE];
    static uint8_t count_board=0;
    while (display_uart_.available()) { //as long as there is data in the port buffer
        display_uart_.read_array(&temp, 1); // we read one byte at a time
        buffer_board[count_board++]=temp; //we collect data in an intermediate buffer, count the amount, do not send it anywhere
        if(temp==0x55 && count_board>5 && (buffer_board[5]+11)<=count_board){ //we received the end of the packet - we send only useful data (we discard 0xAA, CRC and 0x50
           send_packet(buffer_board+3,count_board-8); //the subroutine will generate the package itself
           _debugPrintPacket(buffer_board, count_board, true);
           count_board=0; // clearing the data accumulation buffer
        }            
    }

    // Pipe to display
    static uint8_t buffer_displ[BUFFER_SIZE];
    static uint8_t count_displ=0;
    while (mainboard_uart_.available()) {
        mainboard_uart_.read_array(&temp, 1);
        buffer_displ[count_displ++]=temp;
        if(temp==0x55 && count_displ>5 && (buffer_displ[5]+11)<=count_displ){ // конец пакета
           display_uart_.write_array(buffer_displ, count_displ);
           _debugPrintPacket(buffer_displ, count_displ, false);
           count_displ=0;
        }
    }

}

void PhilipsSeries5400::dump_config() {
    ESP_LOGCONFIG(TAG, "Philips Series 5400");
    display_uart_.check_uart_settings(115200, 1, uart::UART_CONFIG_PARITY_NONE, 8);
    mainboard_uart_.check_uart_settings(115200, 1, uart::UART_CONFIG_PARITY_NONE, 8);
}

} // namespace philips_series_5400
} // namespace esphome
