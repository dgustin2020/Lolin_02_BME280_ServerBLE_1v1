/*
led_blink.cpp
*/
//#include <header.h>
#include <Arduino.h>
#include <my_define.h>
#include <my_Out_Pin.h>
#include "Led_Blink.h"

/* per verifica via seriale*/
//#define BLINK_LED_DEBUG

Led_Blink::Led_Blink()
{
  //Out_Pin        led_blink;//crea led_blink
  tmr_blink_Lclk = 0;//timer per gestione blink led
  p_tbl_blink = 0;
  led_status = false;
  p_rd_data = 0;
}

/****
 * inizualizza task lampeggio led
****/
void Led_Blink::task_init(uint8_t pin_led, T_Lclk* p_tbl, T_Lclk time)
{
  compare_time = time;
  p_tbl_blink = p_tbl;//set qule blink generare
  p_rd_data = p_tbl;//set ptr leggi dati da tabella
  /* set timer blinck*/
  if (*p_rd_data == 0)
    tmr_blink_Lclk = *p_rd_data++;//set timer a 0 per out fisso
  else
    tmr_blink_Lclk = compare_time + *p_rd_data;//set timer
  /* set stato led*/
  p_rd_data++; //next dato
  led_blink.init(pin_led, *p_rd_data);//set stato led
  led_status = *p_rd_data++;//next dato  
  #ifdef BLINK_LED_DEBUG
  Serial.print("init timer ");
  Serial.println(tmr_blink_Lclk);
  Serial.print("init status ");
  Serial.println(led_status);
  Serial.println("");
  #endif
}

/****
 * inizualizza task lampeggio led
****/
void Led_Blink::task_change(T_Lclk* p_tbl, T_Lclk time)
{
  compare_time = time;
  p_tbl_blink = p_tbl;  //set qule blink generare
  p_rd_data = p_tbl;    //set ptr leggi dati da tabella
  /* set timer blinck*/
  if (*p_rd_data == 0)
    tmr_blink_Lclk = *p_rd_data;//set timer a 0 per out fisso
  else
    tmr_blink_Lclk = compare_time + *p_rd_data;//set timer
  /* set stato led*/
  p_rd_data++; //next dato
  led_blink.set(*p_rd_data);//set stato led
  led_status = *p_rd_data++;//next dato  
  #ifdef BLINK_LED_DEBUG
  Serial.print("change timer ");
  Serial.print(tmr_blink_Lclk);
  Serial.print("  change status ");
  Serial.println(led_status);
  #endif
}
/****
 * task chiamato ogni millisec per gestione lampeggioled
****/
bool Led_Blink::task_run(T_Lclk time)
{
  bool ret = true;
  compare_time = time;
  if (tmr_blink_Lclk){//test tmr impostato per cambio stato led
    ret = false;
    if (compare_time > tmr_blink_Lclk){//tmr impostato, verifica se scduto
      if (*p_rd_data == 0){//test se finita sequenza
        p_rd_data = p_tbl_blink;//fiita, rimetti inizio sequenza
        ret = true;
        #ifdef BLINK_LED_DEBUG
        Serial.println("Restart sequenza");
        #endif
      }
      /* timer scaduto, out stato led e reloa timer*/    
      tmr_blink_Lclk += *p_rd_data;//reload timer
      #ifdef BLINK_LED_DEBUG
      Serial.print("run timer ");
      Serial.println(*p_rd_data);
      #endif
      p_rd_data++;
      led_blink.set(*p_rd_data);//out led
      #ifdef BLINK_LED_DEBUG
      Serial.print("run status ");
      Serial.println(*p_rd_data);
      #endif
      led_status = *p_rd_data;
      p_rd_data++;
    }
  }
  return ret;
}

/****/
void Led_Blink::task_enable(void)
{ flg_task_enable = TRUE; }

/****/
void Led_Blink::task_disable(void)
{ flg_task_enable = FALSE; }

/****/
bool Led_Blink::task_status(void)
{ return flg_task_enable; }
