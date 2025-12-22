/*
my_varaibili_base.cpp
*/
#include <Arduino.h>
#include <my_define.h>
#include "variabili_base.h"

T_Lclk loc_Lclk;
T_Lclk Main_Lclk;
T_Main_Base_offset Main_Base_Offset;
T_Lclk Main_Lclk_Offset;
T_Lclk Temp_Lclk;

/* varaibili ad uso Lcd +  Touch */
char            lingua = ITA;


/**** temporanei ****/
char temp_displ_char[100];
uint temp_displ_int[50];
long temp_displ_long[25];

T_impo_utente const  impostazioni_utente_default = {
  KEY_UTENTE_DEFAULT,
  BACKLIGHT_DELAY_DEFAULT,
  SOUND_ON_KEY_DEFAULT,
  SOND_ENABLE_DEFAULT,
  LINGUA_DEFAULT
};


/*
calcola il checksum del buffer buffer p_from in formato uint16_t
somma i gli n_byte uint8_t che compongono il buffer
*/
uint16_t variabili_calcola_checksum(uint8_t* p_from, uint16_t n_byte)
{
  #ifdef DEBUG_FLASH
  Serial.printf("n_byte =%d\r\n", n_byte);
  Serial.printf("key=%x\r\n",*p_from);
  uint16_t start = n_byte;
  #endif
  uint16_t checksum = 0;
  while( n_byte > 0 ){
    #ifdef DEBUG_CHECKSUM
    Serial.printf("check n_byte=%d - val=%x --- checksum=%d -> ",(start - n_byte), *p_from, checksum);
    #endif
    checksum += *p_from++;
    #ifdef DEBUG_CHECKSUM
    Serial.printf(" checksum =%0x\r\n", checksum);
    #endif
    n_byte--;
  }
  #ifdef DEBUG_CHECKSUM
  Serial.printf("n_byte dopo=%d\r\n", n_byte);
  Serial.printf("checksum=%x\r\n",checksum);
  #endif
  return checksum;
}