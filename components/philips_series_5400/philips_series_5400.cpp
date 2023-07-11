
#include <Arduino.h>
#include <stdarg.h>
#include "esphome/core/log.h"
#include "esphome/core/log.h"
#include "philips_series_5400.h"

//#define NEED_LOG

#define BUFFER_BOARD_SIZE 23 // буфер приема пакета в сторону матери
#define BUFFER_DISPL_SIZE 40 // буфер приема пакета в сторону дисплея

// для рассчета контрольной суммы
#define POLY 0x04c11db7

namespace esphome {
namespace philips_series_5400 {


//Байт 0, Байт 3, Байт 4, Cups_Max
uint8_t CoffePattern[14][4]={
    {0,1,1,2},//Espresso
    {0,1,2,1},//Coffee to go
    {0,2,2,2},//Black Coffee
    {0,2,2,2},//Luingo
    {0,2,2,2},//Сaffe Crema
    {0,2,2,2},//Ristretto
    {1,2,3,2},//Americano
    {2,2,1,2},//Coffee With Milk
    {2,2,2,2},//Latte
    {3,2,2,2},//Milk Coffee to go
    {3,2,2,2},//Latte Macchiato
    {3,2,3,2},//Сappuccino
    {4,0,0,1},//Milk foam
    {5,1,0,1} //Hot water
};

uint32_t crc = 0xffffffff; // буффер рассчета CRC
uint8_t BitReverse[] = 
{
  0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
  0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
  0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
  0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
  0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
  0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
  0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
  0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
  0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
  0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
  0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
  0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
  0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
  0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
  0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
  0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

// старт рассчета CRC
inline void PhilipsSeries5400::start_crc(uint8_t val){
   crc = 0xffffffff; // очищаем буер для следующего рассчета
   add_crc(val);
}
 
//БЫСТРО добавить байт к рассчету crc
inline void PhilipsSeries5400::add_crc(uint8_t val){
    ((uint8_t*)(&crc))[3] ^= BitReverse[val];
    for (uint8_t j = 0; j < 8; j++) {
      if (crc & 0x80000000) {
        crc = (uint32_t)((crc << 1) ^ POLY);
      } else {
        crc <<= 1;
      }
    }
}

//БЫСТРО закончить рассчет контрольной суммы
inline uint32_t PhilipsSeries5400::calc_crc(){ 
    uint32_t t32 = 0;
    ((uint8_t*)(&t32))[0]=BitReverse[((uint8_t*)(&crc))[3]];
    ((uint8_t*)(&t32))[1]=BitReverse[((uint8_t*)(&crc))[2]];
    ((uint8_t*)(&t32))[2]=BitReverse[((uint8_t*)(&crc))[1]];
    ((uint8_t*)(&t32))[3]=BitReverse[((uint8_t*)(&crc))[0]];
    crc = t32 ^ 0xffffffff;
    return crc;
}

// рассчет CRC буфера
inline uint32_t PhilipsSeries5400::calc_crc(uint8_t* data, uint8_t size){
   start_crc(data[0]);
   for(uint8_t i=1; i<size; i++){
      add_crc(data[i]);
   } 
   return calc_crc();  
}

// 30 сивмолов лога
char debugBuff[]= "=> 00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;   "; 
#define printBuff (debugBuff+2)

// преобразование полубайта в символ
inline char hex2str(uint8_t b){
    if(b<0x0A){
       return b+'0';
    }
    return b+('A'-0x0A);   
}

// БЫСТРАЯ публикация массива в строку в формате HEX
void printHexLine(uint8_t* buff, uint8_t size, char del=';'){
   if(size>40) size=40; // ограничиваем выход за пределы буфера печати 
   char* bu=printBuff;
   for(uint8_t i=0; i<size; i++){
      *bu++=del;
      *bu++=hex2str(*buff / 0x10);
      *bu++=hex2str(*buff++ & 0x0F);
   }
   *bu=0;
}

// печать HEX строки в сенсор
void PhilipsSeries5400::pubMess(TextSensor *sens,uint8_t* buff, uint8_t size){
   printHexLine(buff, size, ' ');
   sens->publish_state(printBuff);
}

// БЫСТРАЯ печать HEX строки в debug
void PhilipsSeries5400::_debugPrintPacket(uint8_t* data, uint8_t size, bool in){
   // заполняем время пакета
   char timeBuff[11]={0};
   sprintf(timeBuff, "%010u", millis());
   // формируем преамбулы
   if (in) { // признак входящего пакета
      debugBuff[0]='<';
      debugBuff[1]='=';
   } else {  // признак исходящего пакета
      debugBuff[0]='=';
      debugBuff[1]='>';
   } 
   printHexLine(data, size);
   ESP_LOGE(timeBuff, debugBuff);
}

uint8_t receept[]={0x90,1,0x0A,0,0,0,0,0,0,0,0,0,0}; // буфер рецепта
const uint8_t pmb[]={0xAA,0xAA,0xAA}; // преамбула
const uint8_t post=0x55; // постамбула

// отправка пакета с рассчетом посторением к чертовой матери
void PhilipsSeries5400::send_packet_main(uint8_t* data, uint8_t size){
    uint8_t cs[]={0,0,0,0}; 
    *((uint32_t*)cs)=calc_crc(data,size);
    mainboard_uart_.write_array(pmb, sizeof(pmb));
    mainboard_uart_.write_array(data, size);
    mainboard_uart_.write_array(cs, sizeof(cs));
    mainboard_uart_.write_array(&post,1);
    #ifdef NEED_LOG
       _debugPrintPacket(data, size, true);
    #endif
}

// отправка пакета с рассчетом посторением дисплею
void PhilipsSeries5400::send_packet_displ(uint8_t* data, uint8_t size){
    uint8_t cs[]={0,0,0,0}; 
    *((uint32_t*)cs)=calc_crc(data,size);
    display_uart_.write_array(pmb, sizeof(pmb));
    display_uart_.write_array(data, size);
    display_uart_.write_array(cs, sizeof(cs));
    display_uart_.write_array(&post,1);
    #ifdef NEED_LOG
       _debugPrintPacket(data, size, false);
    #endif
}

void PhilipsSeries5400::coffee_test_validate(){ // показ построения рецепта в тестовом режиме
   if(_debug){
      coffee_build(_product, _grind, _cups, _volume, _milk);
   }
}

void PhilipsSeries5400::coffee_start(uint8_t* data){ // старт приготовления кофе по рецепту
   memcpy(receept+3,data,10);
   if(sens_90!=nullptr){pubMess(sens_90, receept, sizeof(receept));}
   if(sens_product!=nullptr){coffee_show(receept+3);}
}

void PhilipsSeries5400::coffee_build(uint8_t type, uint8_t bean, uint8_t cups, uint16_t vol, uint16_t milk){ // старт приготовления кофе
   //ESP_LOGD("BUILD","%u %u %u %u %u", type, bean, cups, vol, milk);
   uint8_t rec[10]={0};
   if(type>=sizeof(CoffePattern)/sizeof(CoffePattern[0])){
      type=0;
   }
   rec[0]=CoffePattern[type][0];// вид напитка 0
   if(rec[0]==5 || rec[0]==4){ // у не кофе 
      rec[1]=3;// всегда признак "молотый"
   } else {
      rec[1]=bean; // крепость/молотый
   }
   if(cups>CoffePattern[type][3]){  // Контроль чашек, например не у всех есть экстрашот
      rec[2]=CoffePattern[type][3];
   } else {
      rec[2]=cups;
   }
   rec[3]=CoffePattern[type][1];// вид напитка 1
   rec[4]=CoffePattern[type][2];// вид напитка 2
   if(rec[0]==1){//  у американо 
      rec[6]=40; // объем кофе всегда 40 мл
      rec[7]=0; 
      rec[8]=(uint8_t)vol;       // а объем напитка в молоке
      rec[9]=vol/0x100;
      rec[5]=0; // но признак добавления молока - нет
   } else {
      // у остальныых объем кофе
      rec[6]=(uint8_t)vol; 
      rec[7]=vol/0x100; 
      if(rec[0]==0 || rec[0]==1 || rec[0]==4 || rec[0]==5){//  у черных, кипятка и молока нет молока
         if(rec[0]==5){ // только молоко
            rec[5]=2; // признак добавления молока
         } else {
            rec[5]=0; // нет молока
         }
         rec[8]=0; 
         rec[9]=0;
      } else {
         rec[5]=2; // признак добавления молока
         rec[8]=(uint8_t)milk; 
         rec[9]=milk/0x100;
      }
   }
   coffee_start(rec);
}

bool PhilipsSeries5400::coffee_decode(const uint8_t* rec, uint8_t* type, uint8_t* bean, uint8_t* cups, uint16_t* vol, uint16_t* milk){ // старт приготовления кофе
   *type=0;
   *bean=rec[1];
   *cups=rec[2];
   *vol=0;
   *milk=0;
   // поиск рецепта в списке
   while((*type)<(sizeof(CoffePattern)/sizeof(CoffePattern[0]))){
      if(rec[0]==CoffePattern[*type][0] && rec[3]==CoffePattern[*type][1] && rec[4]==CoffePattern[*type][2]){
         break;
      }
      (*type)++;
   }
   if(*type<sizeof(CoffePattern)/sizeof(CoffePattern[0])){
      if(rec[0]==1){ //американо
         *vol=rec[9]*0x100+rec[8]+rec[7]*0x100+rec[6];
      } else { //взбитое молоко, кофе с молоком и без
         *vol=rec[7]*0x100+rec[6];
         if(rec[5]==2){
            *milk=rec[9]*0x100+rec[8];
         }
      }
      return true;
   } else {
       *type=0xFF; // не нашли такого сочетания
   }
   return false;// не опознали
}

void PhilipsSeries5400::coffee_show(uint8_t* data){
   if(sens_product!=nullptr){ // если есть сенсор текстового отображения напитка
      uint8_t type;
      uint8_t bean;
      uint8_t cup;
      uint16_t vol;
      uint16_t milk;
      coffee_decode(data, &type, &bean, &cup, &vol, &milk);
      //ESP_LOGD("DECODE","%d %d %d %d %d",type, bean, cup, vol, milk);
      char buff[20]={0};
      if(type<(sizeof(CoffePattern)/sizeof(CoffePattern[0]))){
         std::string show=product->at(type).value(); // название кофе 
         sprintf(buff," %u ml.", vol); //объем
         show = show + buff;
         if(milk){// объем молока
            sprintf(buff,", milk %u ml.", milk); 
            show = show + buff;
         }
         if(cups!=0){ // количество чашек
            show = show + ", " + cups->at(cup).value();
         } 
         if(type<12){ // помол имеет смвсл только у кофе
            show = show + ", " + grind->at(bean).value();
         }
         sens_product->publish_state(show);
      } else {
         sens_product->publish_state("ERROR");
      }
   }
}

uint8_t main_cnt=0; // последний счетчик пакета FF от матери, мы должны отправлять 90 пакет с этим номером
uint8_t disp_cnt=0; // последний счетчик пакета 90 от экрана, мы должны его подтвердить пакетом FF
uint8_t old_disp_cnt=0;

// отправка пакета 9x с подменой нумерации в сторону матери
void PhilipsSeries5400::send_packet_9X(uint8_t* data, uint8_t size){
   if(disp_cnt<data[1] || data[1]==0){ // запоминаем самый большой счетчик посылки, либо сбрасываем его
      disp_cnt=data[1];
   }
   static uint8_t cnt93=0; // номер пакета 93 для посылки в мать
   static uint8_t cnt91=0; // номер пакета 91 для посылки в мать
   static uint8_t cnt90=0; // номер пакета 90 для посылки в мать
   if(data[0]==0x93){  // пакет 93
      static uint8_t old=0xFF; // буфер сравнения номера пакета от дисплея
      if(old!=data[1]){ // если у пакета изменился счетчик
         old=data[1];
         cnt93=main_cnt;
      }          
      data[1]=cnt93;
   } else if(data[0]==0x91){
      static uint8_t old=0xFF; // буфер сравнения номера пакета от дисплея
      static uint8_t old_main_cnt=0xFF; // буфер сравнения номера пакета от дисплея
      if(old!=data[1] && old_main_cnt!=main_cnt){ // если у пакета изменился счетчик
         old=data[1];
         old_main_cnt=main_cnt;
         cnt91=main_cnt;
         if(cnt91==cnt93){ // если счетчик равен, то изменяем его
            if(data[3]==0x03){ // приготовление кофе
               cnt91=cnt93+2;
            } else if(data[3]==0x10){
               cnt91=cnt93+1;
            }              
         }
      }          
      data[1]=cnt91;
   } else if(data[0]==0x90){
      static uint8_t old=0xFF; // буфер сравнения номера пакета от дисплея
      if(old!=data[1]){ // если у пакета изменился счетчик
         old=data[1];
         cnt90=cnt93+1;
      }          
      data[1]=cnt90;
   }
   //ESP_LOGD("9X","%x -> %x", main_cnt, data[1]);
   send_packet_main(data,size); // отправляем матери пакет c номером полученным от матери
}

// отправка пакета 9x из конфига
void PhilipsSeries5400::send_packet_90(uint8_t* data, uint8_t size){
   //ESP_LOGD("90","%x->%x", main_cnt, data[1]);
   send_packet_main(data,size); // отправляем пакет с рассчетом
}

// отправка пакета FF c восстановленой нумерацией для успокоения дисплея
void PhilipsSeries5400::send_packet_FF(uint8_t* data, uint8_t size){ // обрабатываем этот пакет в сторону дисплея, он связан с 9X
   main_cnt=data[1]; // запоминаем номер который нам продиктовала мать для следующего пакета
   // корректируем ответ для дисплея
   if(old_disp_cnt!=disp_cnt+1){// счетчик в ответе должен быть на 1 больше счетчика от дисплея
      old_disp_cnt++;
   } else if(data[1]==0){ // выключение
      old_disp_cnt=0;
      disp_cnt=0;
   }
   data[1]=old_disp_cnt;
   data[3]=data[1];
   //ESP_LOGD("FF","%x -> %x", main_cnt, data[1]);
   send_packet_displ(data,size); // отправляем пакет с рассчетом
}

// публикация статуса
void PhilipsSeries5400::pubStat(uint8_t stat){
   if(work_sensor_!=nullptr){ // только если подключен сенсор
      static uint8_t old=0;
      if(stat!=0 && stat!=old){ // запрет публикации повторных статусов
         old=stat;
         work_sensor_->publish_state(stat); 
      }
   }
}

// проверка и при необходимости сохранение данных
void PhilipsSeries5400::checkStore(){
   if(_need_store){
      bool save=false;
      if(stored_data.volume!=_volume){stored_data.volume=_volume; save=true;}
      if(stored_data.milk!=_milk){stored_data.milk=_milk; save=true;}
      if(stored_data.grind!=_grind){stored_data.grind=_grind; save=true;}
      if(stored_data.cups!=_cups){stored_data.cups=_cups; save=true;}
      if(stored_data.product!=_product){stored_data.product=_product; save=true;}
      if(save){ //сохраняем данные
         if (!storage.save(&stored_data) || !global_preferences->sync()) {
            ESP_LOGE(TAG,"Store data ERROR");
         } else {
            ESP_LOGD(TAG,"Store presset data to FLASH");
         }
      }
   }
}

// восстановлениеданных из флеша
void PhilipsSeries5400::reStore(){
   if(_need_store){
      if(storage.load(&stored_data)==1){// читаем все пресеты из флеша
         ESP_LOGD(TAG,"Load preset from FLASH");
         volume->publish_state(stored_data.volume);
         milk->publish_state(stored_data.milk);
         cups->publish_state(cups->at(stored_data.cups).value());
         grind->publish_state(grind->at(stored_data.grind).value());
         product->publish_state(product->at(stored_data.product).value());
         
      } else {
         ESP_LOGE(TAG,"Load preset from FLASH - ERROR"); 
      }
   }      
}

void PhilipsSeries5400::setup(){
   reStore();
}

void PhilipsSeries5400::loop() {

    uint32_t _now=millis(); // текущее системное время
    static uint32_t on_control=_now; // время последней активности обмена данными
    static uint32_t coffee_ready=1; //время окончания приготовления последнего кофе
    uint8_t temp; // буфер для считывания байта из порта
    
    { // Pipe display to mainboard
       static uint8_t buffer_board[BUFFER_BOARD_SIZE];
       static uint8_t count_board=0;
       while (display_uart_.available() && display_uart_.read_byte(&temp)){
           if((count_board>2) || (temp==0xAA /* && count_board>=0 && count_board<=2*/)) { //ждем начало пакета
              buffer_board[count_board++]=temp; //собираем данные в промежуточный буфер, считаем количество, никуда не отправляем
              if(count_board>=BUFFER_BOARD_SIZE){ // ограничение переполнения буфера
                 count_board=0;
              }
           } else {
              count_board=0;
           }
           if(temp==0x55 && count_board>5 && ((buffer_board[5]+11)<=count_board)){ // получили конец пакета - отправляем только полезные данные (отбрасываем 0xAA, CRC и 0x50
              if((buffer_board[3] & 0xF0)==0x90){ // пакет 9х
                 send_packet_9X(buffer_board+3, count_board-8);
                 static uint8_t in_counter=0;
                 if( buffer_board[4]<5){ // первые пакеты не контролим
                    in_counter=buffer_board[4];
                 } else if(in_counter<buffer_board[4]){ // защита от повторов
                    in_counter=buffer_board[4];
                    if(buffer_board[3]==0x90){
                       if(sens_product!=nullptr){coffee_show(buffer_board+6);}  //  текстовая расшифровка рецепта
                       if(sens_90!=nullptr){ pubMess(sens_90, buffer_board+3, count_board-8);}
                    } else if(buffer_board[3]==0x91){
                       if(buffer_board[6]==0x08){ //конец приготовления кофе
                          coffee_ready=millis(); //засекли момент окончания приготовления кофе   
                       }
                       if(sens_91!=nullptr){ pubMess(sens_91, buffer_board+3, count_board-8);}
                    } else if(buffer_board[3]==0x93){
                       coffee_ready=0; // старт приготовления кофе
                       if(sens_93!=nullptr){ pubMess(sens_93, buffer_board+3, count_board-8);}
                    }
                 } 
              } else {
                 mainboard_uart_.write_array(buffer_board,count_board); // простая трансляция пакета
                 #ifdef NEED_LOG
                    _debugPrintPacket(buffer_board+3, count_board-8, true);
                 #endif
              }
              on_control=_now; // запомнить время активности протокола
              count_board=0; // очищаем буфер накопления данных 
              break;
           }
       }
    }

    { // Pipe to display
       static uint8_t buffer_displ[BUFFER_DISPL_SIZE];
       static uint8_t count_displ=0;
       while (mainboard_uart_.available() && mainboard_uart_.read_byte(&temp)){
           if(count_displ>2 || temp==0xAA) { //ждем начало пакета
              buffer_displ[count_displ++]=temp; //собираем данные в промежуточный буфер, считаем количество, никуда не отправляем
              if(count_displ>=BUFFER_DISPL_SIZE){ // ограничение переполнения буфера
                 count_displ=0;
              }
           } else {
              count_displ=0;
           }
           if(temp==0x55 && count_displ>5 && ((buffer_displ[5]+11)<=count_displ)){ // конец пакета и пакет в порядке
              if(buffer_displ[3]==0xFF){ // пакет FF
                send_packet_FF(buffer_displ+3, count_displ-8); // обрабатываем этот пакет, он связан с 9X
              } else { // остальные пакеты просто дублируем
                display_uart_.write_array(buffer_displ,count_displ); // простая трансляция пакета
                #ifdef NEED_LOG
                    _debugPrintPacket(buffer_displ+3, count_displ-8, false);
                #endif
                if(buffer_displ[3]==0xFE){ // пакет вкл-выкл
                   disp_cnt=0;
                   old_disp_cnt=0;
                   //ESP_LOGE("","RESET CONTROL COUNTERS");
                   #ifdef NEED_LOG
                       //_debugPrintPacket(buffer_displ+3, count_displ-8, false);
                   #endif
                } else if((buffer_displ[3] & 0xF0) ==0xB0){ // пакет Bx
                   static uint8_t in_counter=0;
                   if(in_counter+1<=buffer_displ[4] || in_counter-buffer_displ[4]>5){ // защита от повторов
                      in_counter=buffer_displ[4];
                      if(buffer_displ[3]==0xB0){ //AA:AA:AA:B0 "Статусы"
                         if(buffer_displ[6]==0x0E){
                            if(buffer_displ[9]==0x00){pubStat(20);} //"Опорож. контейнер для коф. гущи"
                            else {
                               if((buffer_displ[9] & 0x40)!=0){pubStat(22);} //"Воды нет"
                               else {pubStat(17);} //"Вода есть"
                               if((buffer_displ[9] & 0x80)!=0){pubStat(23);} //"Извлечен"
                               else {pubStat(18);} //"Вставлен"
                            }
                         } else if(buffer_displ[6]==0x06){
                            pubStat(21);   //"Вода есть","Вставлен","Зерна есть","Выберите напиток","Пустой";                  
                         } else if(buffer_displ[6]==0x0C){
                            if     (buffer_displ[7]==0x01){pubStat(8);} //"Наслаждайтесь"
                            else if(buffer_displ[7]==0x02){pubStat(15);} //"Что-то (07 0C 02)"
                         } else if(buffer_displ[6]==0x07){                    
                            if     (buffer_displ[7]==0x0E){pubStat(9);} //"Нагрев воды"
                            else if(buffer_displ[7]==0x0D){pubStat(10);} //"Перемалываем зерна"
                            else if(buffer_displ[7]==0x10){pubStat(3);} //"Наливаем молоко"
                            else if(buffer_displ[7]==0x11){pubStat(4);} //"Наливаем кофе"
                            else if(buffer_displ[7]==0x12){pubStat(5);} //"Предварительное дозирование"
                            else if(buffer_displ[7]==0x13){pubStat(6);} //"Создание пара для молока"
                            else if(buffer_displ[7]==0x14){pubStat(7);} //"Заварочный узел в положение заваривания"
                            else if(buffer_displ[7]==0x15){pubStat(15);} //"Наслаждайтесь"
                         } else if(buffer_displ[6]==0x08){
                            if     (buffer_displ[7]==0x0E){pubStat(14);} //"Нагревание"
                            else if(buffer_displ[7]==0x02){pubStat(11);} //"Промывка"
                            else if(buffer_displ[7]==0x14){pubStat(13);} //"Что-то (07 08 14)"
                            else if(buffer_displ[7]==0x05){pubStat(12);} //"Зерна закончились"
                            else if(buffer_displ[7]==0x16){pubStat(24);} //"Удаление накипи стадия 1"
                            else if(buffer_displ[7]==0x18){pubStat(25);} //"даление накипи стадия 2"
                            
                         } else if(buffer_displ[7]==0x00){
                            if     (buffer_displ[6]==0x01){pubStat(1);} //"Что-то (07 01 00)"
                            else if(buffer_displ[6]==0x05){pubStat(2);} //"Выключено"
                         }
                         // если подключен сенсор печати пакета b0, то публикуем его
                         if(sens_b0!=nullptr){ pubMess(sens_b0, buffer_displ, count_displ);}
                      } else if(buffer_displ[3]==0xB5){ //AA:AA:AA:B5 "Error Code"
                         if(buffer_displ[10]==0x00){
                            if     (buffer_displ[11]==0x00){pubStat(30);} //"0x00"
                            else if(buffer_displ[11]==0x0B){pubStat(31);} //"0x0B"
                            else if(buffer_displ[11]==0xE6){pubStat(32);} //"0xE6"
                            else if(buffer_displ[11]==0x80){pubStat(33);} //"0x80"
                            else if(buffer_displ[11]==0xCB){pubStat(34);} //"0xCB"
                            else if(buffer_displ[11]==0xFF){pubStat(35);} //"0xFF"
                            else if(buffer_displ[11]==0xA0){pubStat(36);} //"0xA0"
                         } else if(buffer_displ[10]==0x01){ pubStat(37);} // "Статус2 0x01"
                         // если подключен сенсор печати пакета b5, то публикуем его
                         if(sens_b5!=nullptr){ pubMess(sens_b5, buffer_displ, count_displ);}
                      } else { //AA:AA:AA:Bx "Все остальные Bx" 
                         // если подключен сенсор печати пакета ba, то публикуем его
                         if(sens_ba!=nullptr){ pubMess(sens_b0, buffer_displ, count_displ);}
                      }
                   }
                } else { // все остальное
                   #ifdef NEED_LOG
                      _debugPrintPacket(buffer_displ+3, count_displ-8, false);
                   #endif
                }
              }
              count_displ=0; // пакет обработан - освободить буфер
              on_control=_now; // запомнить время активности протокола
              break;
           }
       }
    }
    
    // контроль активности протокола 
    if(_now-on_control>15000){ // нет обмена более 15 сек
       pubStat(2); // публикуем статус - машинка выключена
       on_control=_now;
    }
    
    static uint32_t sendTimer=0;
    // приготовление кофе
    
    if(coffe_route && coffee_ready  && _now-sendTimer>12 && _now-coffee_ready>1000){ // запускаем только после окончания приготовления педыдущего стакана
       static uint8_t pack93[]={0x93,0,1,1};
       static uint8_t pack91[]={0x91,2,1,3};
       if(coffe_route==1){ // построение пакетов
           //ESP_LOGE("","START COFFEE ESP");
           pack93[1]=main_cnt;
           receept[1]=main_cnt+1;
           pack91[1]=main_cnt+2;
           send_packet_90(pack93, sizeof(pack93));
           coffe_route++;
       } else if(coffe_route==4 || coffe_route==7){ //
           send_packet_90(pack93, sizeof(pack93));
           coffe_route++;
       } else if(coffe_route==2){
           send_packet_90(receept, sizeof(receept));
           if(sens_90!=nullptr){pubMess(sens_90, receept, sizeof(receept));}
           coffe_route++;
       } else if(coffe_route==5 || coffe_route==8){
           send_packet_90(receept, sizeof(receept));
           coffe_route++;
       } else if(coffe_route==3 || coffe_route==6){
           send_packet_90(pack91, sizeof(pack91));
           coffe_route++;
       } else if(coffe_route==9){
           send_packet_90(pack91, sizeof(pack91));
           coffe_route=0;
           coffee_ready=0; // конец старта приготовления кофе
           //ESP_LOGE("","END COFFEE ESP");
       }
       sendTimer=_now;
    }

    // контроль необходимости сохранения данных
    static uint32_t saveTimer=0;
    if(_need_store && _now-saveTimer>1000){
       saveTimer=_now;
       checkStore();
    }
        
}

void PhilipsSeries5400::dump_config() {
    ESP_LOGCONFIG(TAG, "Philips Series 5400");
    display_uart_.check_uart_settings(115200, 1, uart::UART_CONFIG_PARITY_NONE, 8);
    mainboard_uart_.check_uart_settings(115200, 1, uart::UART_CONFIG_PARITY_NONE, 8);
    LOG_SENSOR(TAG, "Status sensor ", this->work_sensor_);
    LOG_TEXT_SENSOR(TAG, "Message 90 sensor", this->sens_90); 
    LOG_TEXT_SENSOR(TAG, "Message 91 sensor", this->sens_91); 
    LOG_TEXT_SENSOR(TAG, "Message 93 sensor", this->sens_93); 
    LOG_TEXT_SENSOR(TAG, "Message B0 sensor", this->sens_b0); 
    LOG_TEXT_SENSOR(TAG, "Message B5 sensor", this->sens_b5); 
    LOG_TEXT_SENSOR(TAG, "Message BA sensor", this->sens_ba); 
    LOG_TEXT_SENSOR(TAG, "Drink composition", this->sens_product); 
    LOG_NUMBER(TAG, "Coffee size", this->volume); 
    LOG_NUMBER(TAG, "Milk size", this->milk);  
    LOG_SELECT(TAG, "Select drink", this->product);
    LOG_SELECT(TAG, "Select grind", this->grind);
    LOG_SELECT(TAG, "Select cups", this->cups);
    LOG_BUTTON(TAG, "Prepare button", this->start_button);
    ESP_LOGCONFIG(TAG, "Save settings %s", TRUEFALSE(_need_store));
    ESP_LOGCONFIG(TAG, "Debug %s", TRUEFALSE(_debug));
}

} // namespace philips_series_5400
} // namespace esphome
