/*
itr_time.c
uso il timer 0 di 4
*/
//#include <common_include.h>
#include <Arduino.h>
#include <Preferences.h>
#include <my_define.h>
#include <my_lib_clock_long.h>

#include <..\..\src\preferenze_gest\pref_gest.h>
//#include <../../src/Flash_param/Flash_param.h>

#include "my_itr_time.h"

#define SET_CAL

/* clock per generazione timer locali incrementato ad ogni chiamat del itr a tempo */
volatile T_Lclk Base_Lclk;

/* locali */
static bool itr_time_scaduto_flg = false;
static hw_timer_t * timer = NULL;
static portMUX_TYPE timerMux_itr_time = portMUX_INITIALIZER_UNLOCKED;
static uint16_t cnt_reload;
static uint64_t itr_compare_buff;
#ifndef SET_CAL
static uint64_t itr_n_cicli;
#endif

static FnPtr gestione_itr;//procedura chiamata in itr

/****  Code with critica section ****/
void IRAM_ATTR onTime_itr(void) {
  portENTER_CRITICAL_ISR(&timerMux_itr_time);

  #ifdef SET_CAL
  /* gestione calibrazione timer */
  cnt_reload++;
  if (cnt_reload > 999)
    cnt_reload = 0;
  if (cnt_reload > base_param.xtal_sel)
    itr_compare_buff += (uint32_t)base_param.xtal_base;         
  else 
    itr_compare_buff += (uint32_t)base_param.xtal_base_1;    
  #else
  itr_compare_buff +=  itr_n_cicli;
  #endif   
  timerAlarmWrite(timer,(uint64_t)itr_compare_buff, false);   //riarma compare register  
  itr_time_scaduto_flg = true;  /* set flag itr scaduto per main */
  //////gestione_itr();  /* goto a gestione itr */
  
  portEXIT_CRITICAL_ISR(&timerMux_itr_time);
}
  

/****/
void itr_time_init(FnPtr gest_itr, uint8_t timer_usato, uint16_t divisore_usato, uint64_t n_cicli) {
  gestione_itr = gest_itr;
  #ifndef SET_CAL
  itr_n_cicli = n_cicli;
  #endif
  #ifdef DEBUG_CAL_XTAL
  Serial.printf("itr_time_1000_init\r");
  Serial.printf("cal.base    = %0d-%0d\r", (uint32_t)(calset.cal_base >> 32), (uint32_t)calset.cal_base);
  Serial.printf("cal.base_1  = %0d-%0d\r", (uint32_t)(calset.cal_base_1 >> 32), (uint32_t)calset.cal_base_1);
  Serial.printf("cal.sel     = %0d\r", calset.cal_sel);
  Serial.printf("cal.default = %d\r\n", calset.cal_default);
  #endif
  
  // Configure the Prescaler at 80 the quarter of the ESP32 is cadence at 80Mhz ---> 80000000 / 16 = 5000000 tics / seconde
  timer = timerBegin(timer_usato, divisore_usato, true);                
  timerAttachInterrupt(timer, &onTime_itr, true);    
  timerStop(timer);
  itr_compare_buff = timerRead(timer);
  #ifdef SET_CAL
  itr_compare_buff += (uint32_t)base_param.xtal_base_1;  //TIC_TIMER_1MSEC;
  #else
  itr_compare_buff += TIC_TIMER_1MSEC;
  #endif
  cnt_reload = 0;
  timerAlarmWrite(timer, itr_compare_buff, false);           
  timerAlarmEnable(timer);    
  timerStart(timer);
}

uint64_t itr_time_read_counter(void)
{
  return timerRead(timer);
}

bool itr_time_scaduto(void)
{
  return itr_time_scaduto_flg;
}

void itr_time_scaduto_clear(void)
{
  itr_time_scaduto_flg = false;
}